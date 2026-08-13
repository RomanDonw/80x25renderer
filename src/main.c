#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#include "shaders.h"

static const char *wintitle = "80x25 CGA Display";

static void frbuffresz_callback(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

static bool isshadercompilationsuccessful(GLuint shader);
static GLchar *getshadercompilelog(GLuint shader);

int main(int argc, char *argv[])
{
    int winw = 800, winh = 600;
    {
        int p;
        while ((p = getopt(argc, argv, "w:h:")) != -1)
        {
            switch (p)
            {
                case 'w':
                    if (sscanf(optarg, "%d", &winw) < 1)
                    { puts("error parsing decimal integer in option -w"); return 1; }
                    break;

                case 'h':
                    if (sscanf(optarg, "%d", &winh) < 1)
                    { puts("error parsing decimal integer in option -h"); return 1; }
                    break;
            }
        }
    }

    //printf("window width: %i, window height: %i\n", winw, winh);

    if (!glfwInit()) { puts("failed to initialize GLFW"); return 1; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *w;
    if (!(w = glfwCreateWindow(800, 600, "80x25 CGA renderer", NULL, NULL)))
    { puts("error creating window"); glfwTerminate(); return 1; }
    glfwMakeContextCurrent(w);

    if (!gladLoadGL(glfwGetProcAddress))
    { puts("error loading OpenGL context through GLAD"); glfwTerminate(); return 1; }

    //glViewport(0, 0, winw, winh);
    glfwSetFramebufferSizeCallback(w, frbuffresz_callback);

    // ==========================================================================================
    
    {
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexshadersource, NULL);
        glCompileShader(vs);
        if (!isshadercompilationsuccessful(vs))
        {
            char *log = getshadercompilelog(vs);
            printf("error compiling vertex shader:\n%s\n");
            free(log);
            glfwTerminate();
            return 1;
        }

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentshadersource, NULL);
        glCompileShader(fs);
        if (!isshadercompilationsuccessful(fs))
        {
            char *log = getshadercompilelog(fs);
            printf("error compiling fragment shader:\n%s\n");
            free(log);
            glfwTerminate();
            return 1;
        }

        GLuint prog = glCreateProgram();
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);
        glUseProgram(prog);

    }

    // ==========================================================================================

    glClearColor(0.0, 0.0, 0.0, 1.0);
    while (!glfwWindowShouldClose(w))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        

        glfwSwapBuffers(w);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
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
