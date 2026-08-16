#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <libmonotime.h>

#include "shaders.h"
#include "renderfuncs.h"

static unsigned int fbwidth, fbheight;
static void frbuffresz_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    fbwidth = width;
    fbheight = height;
}

static bool isshadercompilationsuccessful(GLuint shader);
static GLchar *getshadercompilelog(GLuint shader);
static void applytex2Dlinearfilts(void);

#define GLDEBUG() (printf("OpenGL error %u at %llu:%s in function %s\n", glGetError(), __LINE__, __FILE__, __func__))

int main(int argc, char *argv[])
{
    if (!monotime_now(NULL)) { puts("monotonic system timer doesn't supported on this platform"); return 1; }

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
    { puts("error creating window"); free(fontbitmap); goto errorquit_afterinitglfw; }
    glfwMakeContextCurrent(w);

    if (!gladLoadGL(glfwGetProcAddress))
    { puts("error loading OpenGL context through GLAD"); free(fontbitmap); goto errorquit_afterinitglfw; }

    glfwSetWindowSizeLimits(w, 80 * 8, 25 * 16, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetFramebufferSizeCallback(w, frbuffresz_callback);

    // ==========================================================================================

    GLuint font;
    glGenTextures(1, &font);
    glBindTexture(GL_TEXTURE_2D, font);
    applytex2Dlinearfilts();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, 16, 256, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, fontbitmap);
    free(fontbitmap);
    
    uint16_t *vmemdata = malloc(80 * 25 * 2);
    if (!vmemdata) { puts("error allocating video memory buffer in RAM"); goto errorquit_afterinitglfw; }
    memset(vmemdata, 0, 80 * 25 * 2);
    
    vmem_clear(vmemdata, 7);
    vmem_writecs(vmemdata, 0, "Microsoft(R) MS-DOS(R) Version 6.22");
    vmem_writecs(vmemdata, 80 + 13, "(C)Copyright Microsoft Corp 1981-1994");
    vmem_writecs(vmemdata, 80 * 3, "C:\\DOS>");

    GLuint vmem;
    glGenTextures(1, &vmem);
    glBindTexture(GL_TEXTURE_2D, vmem);
    applytex2Dlinearfilts();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8UI, 80, 25, 0, GL_RG_INTEGER, GL_UNSIGNED_BYTE, vmemdata);

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
            goto errorquit_aftervmemalloc;
        }

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentshadersource, NULL);
        glCompileShader(fs);
        if (!isshadercompilationsuccessful(fs))
        {
            char *log = getshadercompilelog(fs);
            printf("error compiling fragment shader:\n%s\n", log);
            free(log);
            goto errorquit_aftervmemalloc;
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
                goto errorquit_aftervmemalloc;
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

    const float colors[] =
    {
        0, 0, 0, // black.
        0, 0, 0xAA / 255.0, // dark blue.
        0, 0xAA / 255.0, 0, // dark green.
        0, 0xAA / 255.0, 0xAA / 255.0, // dark cyan.
        0xAA / 255.0, 0, 0, // dark red.
        0xAA / 255.0, 0, 0xAA / 255.0, // dark magenta.
        0xAA / 255.0, 0x55 / 255.0, 0, // dark yellow/brown.
        0xAA / 255.0, 0xAA / 255.0, 0xAA / 255.0, // gray.

        0x55 / 255.0, 0x55 / 255.0, 0x55 / 255.0, // dark gray.
        0x55 / 255.0, 0x55 / 255.0, 1, // light blue.
        0x55 / 255.0, 1, 0x55 / 255.0, // light green.
        0x55 / 255.0, 1, 1, // light cyan.
        1, 0x55 / 255.0, 0x55 / 255.0, // light red.
        1, 0x55 / 255.0, 1, // light magenta.
        1, 1, 0x55 / 255.0, // light yellow.
        1, 1, 1 // light white.
    };
    glUniform3fv(glGetUniformLocation(prog, "colors"), 16, colors);

    glUniform1i(glGetUniformLocation(prog, "vmem"), 0);
    glActiveTexture(GL_TEXTURE0);

    glUniform1i(glGetUniformLocation(prog, "font"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, font);

    GLint u_textblinkstate = glGetUniformLocation(prog, "textblinkstate");

    GLint u_curenabled = glGetUniformLocation(prog, "curenabled");
    GLint u_curpos = glGetUniformLocation(prog, "curpos");
    GLint u_curbounds = glGetUniformLocation(prog, "curbounds");
    GLint u_curblinkstate = glGetUniformLocation(prog, "curblinkstate");

    glUniform1ui(u_curenabled, true);
    glUniform2ui(u_curpos, 7, 3);
    glUniform2ui(u_curbounds, 14, 15);

    monotime_t currtime;
    while (!glfwWindowShouldClose(w))
    {   
        if (!monotime_now(&currtime)) { puts("error getting monotonic time in render loop"); break; }
        glUniform1ui(u_curblinkstate, (bool)((currtime / 266666667) % 2));
        glUniform1ui(u_textblinkstate, (bool)((currtime / 533333333) % 2));

        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, NULL);

        skipdraw:
        glfwSwapBuffers(w);
        glfwPollEvents();
    }

    free(vmemdata);
    glfwTerminate();
    return 0;

    errorquit_aftervmemalloc:
        free(vmemdata);
    errorquit_afterinitglfw:
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

static void applytex2Dlinearfilts(void)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}