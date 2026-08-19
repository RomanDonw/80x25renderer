/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "tmrenderer.h"

#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <libmonotime.h>

#include "shaders.h"

#define MINWINWIDTH 640
#define MINWINHEIGHT 400

static bool inited = false;
static GLFWwindow *w = NULL;
static GLuint font = 0, vvmem = 0, curshape = 0, prog = 0;

static bool isshadercompilationsuccessful(GLuint shader);
static void applytexlinearfilts(GLenum target);
static void onresize(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

static const float vertices[] =
{
    -1.0, -1.0,
    1.0, -1.0,
    -1.0, 1.0,
    1.0, 1.0
};
static const unsigned int indices[] =
{
    0, 1, 3,
    0, 2, 3
};

NError tmrenderer_init(const char *title)
{
    if (inited) return NError_AlreadyInitialized;
    if (!monotime_now(NULL)) goto errorquit_afterinitglfw;

    if (!glfwInit()) goto errorquit_generic;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!(w = glfwCreateWindow(MINWINWIDTH, MINWINHEIGHT, title, NULL, NULL))) goto errorquit_afterinitglfw;
    glfwMakeContextCurrent(w);

    if (!gladLoadGL(glfwGetProcAddress)) goto errorquit_afterinitglfw;

    glfwSetWindowSizeLimits(w, MINWINWIDTH, MINWINHEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetFramebufferSizeCallback(w, onresize);
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // ==========================================================================================

    glGenTextures(1, &font);
    glBindTexture(GL_TEXTURE_2D, font);
    applytexlinearfilts(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, 16, 256, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);

    glGenTextures(1, &vvmem);
    glBindTexture(GL_TEXTURE_2D, vvmem);
    applytexlinearfilts(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8UI, 80, 25, 0, GL_RG_INTEGER, GL_UNSIGNED_BYTE, NULL);

    glGenTextures(1, &curshape);
    glBindTexture(GL_TEXTURE_1D, curshape);
    applytexlinearfilts(GL_TEXTURE_1D);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R8UI, 16, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);

    // ==========================================================================================

    {
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexshadersource, NULL);
        glCompileShader(vs);
        if (!isshadercompilationsuccessful(vs)) goto errorquit_afterinitglfw;

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentshadersource, NULL);
        glCompileShader(fs);
        if (!isshadercompilationsuccessful(fs)) goto errorquit_afterinitglfw;

        prog = glCreateProgram();
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);
        glUseProgram(prog);

        GLint result;
        glGetProgramiv(prog, GL_LINK_STATUS, &result);
        if (!result) goto errorquit_afterinitglfw;
    }

    // ==========================================================================================

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

    inited = true;
    return NError_Success;

    errorquit_afterinitglfw:
        glfwTerminate();
    errorquit_generic:
    return NError_Fault;
}

NError tmrenderer_quit(void)
{
    if (!inited) return NError_NotInitialized;

    glfwTerminate();
    w = NULL;
    font = vvmem = curshape = prog = 0;

    inited = false;
    return NError_Success;
}

static bool isshadercompilationsuccessful(GLuint shader)
{
    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    return result;
}

/*
static char *getshadercompilelog(GLuint shader)
{
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length <= 0) return NULL;

    char *ret = malloc(length);
    glGetShaderInfoLog(shader, length, NULL, ret);
    return ret;
}
*/

static void applytexlinearfilts(GLenum target)
{
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}