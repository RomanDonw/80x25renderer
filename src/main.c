#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <stddef.h>

#include "shaders.h"

static unsigned int fbwidth, fbheight;
static void frbuffresz_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    fbwidth = width;
    fbheight = height;
}

static bool isshadercompilationsuccessful(GLuint shader);
static GLchar *getshadercompilelog(GLuint shader);

int main(int argc, char *argv[])
{
    void *fontbitmap = NULL;
    int winw = 800, winh = 600;

    {
        int p;
        while ((p = getopt(argc, argv, "w:h:f:")) != -1)
        {
            switch (p)
            {
                case 'w':
                    if (sscanf(optarg, "%d", &winw) < 1)
                    { puts("error parsing decimal integer in option -w"); free(fontbitmap); return 1; }
                    break;

                case 'h':
                    if (sscanf(optarg, "%d", &winh) < 1)
                    { puts("error parsing decimal integer in option -h"); free(fontbitmap); return 1; }
                    break;

                case 'f':
                    FILE *f = fopen(optarg, "rb");
                    if (!f) { printf("error opening bitmap font file \"%s\"\n", optarg); return 1; }

                    if (!(fontbitmap = malloc(16 * 256)))
                    { puts("memory allocation failed"); fclose(f); return 1; }

                    fread(fontbitmap, 1, 16 * 256, f);

                    fclose(f);
                    break;
            }
        }
    }

    if (!fontbitmap)
    {
        puts("specify bitmap font file through -f parameter");
        return 1;
    }

    // ==========================================================================================

    if (!glfwInit()) { puts("failed to initialize GLFW"); free(fontbitmap); return 1; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *w;
    if (!(w = glfwCreateWindow(800, 600, "80x25 CGA renderer", NULL, NULL)))
    { puts("error creating window"); free(fontbitmap); goto errorquit_glfw; }
    glfwMakeContextCurrent(w);

    if (!gladLoadGL(glfwGetProcAddress))
    { puts("error loading OpenGL context through GLAD"); free(fontbitmap); goto errorquit_glfw;  }

    glfwSetFramebufferSizeCallback(w, frbuffresz_callback);

    // ==========================================================================================
    
    GLuint font;
    glGenTextures(1, &font);
    glBindTexture(GL_TEXTURE_1D, font);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, 16 * 256, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, fontbitmap);
    free(fontbitmap);

    // ==========================================================================================
    
    GLuint prog;
    {
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexshadersource, NULL);
        glCompileShader(vs);
        if (!isshadercompilationsuccessful(vs))
        {
            char *log = getshadercompilelog(vs);
            printf("error compiling vertex shader:\n%s\n", log);
            free(log);
            goto errorquit_glfw;
        }

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentshadersource, NULL);
        glCompileShader(fs);
        if (!isshadercompilationsuccessful(fs))
        {
            char *log = getshadercompilelog(fs);
            printf("error compiling fragment shader:\n%s\n", log);
            free(log);
            goto errorquit_glfw;
        }

        prog = glCreateProgram();
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);
        glUseProgram(prog);

        GLint result;
        glGetProgramiv(prog, GL_LINK_STATUS, &result);
        if (!result)
        {
            GLint length;
            glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);
            if (length > 0)
            {
                char *log = malloc(length);
                if (log)
                {
                    glGetProgramInfoLog(prog, length, NULL, log);
                    printf("error linking shader program:\n%s\n", log);
                    free(log);
                }
                goto errorquit_glfw;
            }
        }

    }

    // ==========================================================================================

    const float vertices[] =
    {
        -1.0, -1.0,
        1.0, -1.0,
        -1.0, 1.0,
        1.0, 1.0
    };
    const unsigned int indices[] =
    {
        0, 1, 3,
        0, 2, 3
    };
    
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // ==========================================================================================
    
    GLint unfs_screensize = glGetUniformLocation(prog, "scrsize");
    //if (unfs_screensize < 0) { puts("unable to get location of uniform variable \"scrsize\""); goto errorquit_glfw; }

    GLint unfs_cellsize = glGetUniformLocation(prog, "cellsize");
    //if (unfs_cellsize < 0) { puts("unable to get location of uniform variable \"cellsize\""); goto errorquit_glfw; }
    
    GLint unfs_fontscalefactor = glGetUniformLocation(prog, "fontscalefactor");

    glUniform1i(glGetUniformLocation(prog, "font"), 0);
    glBindTexture(GL_TEXTURE_1D, font);
    glActiveTexture(GL_TEXTURE0);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    while (!glfwWindowShouldClose(w))
    {
        register unsigned long long cellwidth = fbwidth / 80;
        register unsigned long long cellheight = fbheight / 25;
        //printf("%llu:%llu\n", cellwidth, cellheight);
        if (!cellwidth || !cellheight) goto skipdraw;

        glUniform2ui(unfs_screensize, fbwidth, fbheight);
        glUniform2ui(unfs_cellsize, cellwidth, cellheight);
        glUniform2ui(unfs_fontscalefactor, cellwidth / 8, cellheight / 16);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, NULL);

        skipdraw:
        glfwSwapBuffers(w);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

    errorquit_glfw:
        glfwTerminate();
    return 1;
}

static bool isshadercompilationsuccessful(GLuint shader)
{
    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    return result;
}

static char *getshadercompilelog(GLuint shader)
{
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length <= 0) return NULL;

    char *ret = malloc(length);
    glGetShaderInfoLog(shader, length, NULL, ret);
    return ret;
}
