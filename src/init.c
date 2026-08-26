/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#define INIT_C
#include "init.h"

#include <string.h>

#include "shaders.h"
#include "util.h"
#include "constants.h"
#include "text.h"
#include "cursor.h"

#define MINWINWIDTH 640
#define MINWINHEIGHT 400

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

struct ctxn_s __libtmrenderer_ctxn = {0};
struct ctxu_s __libtmrenderere_ctxu = {-1};

bool __libtmrenderer_inited = false;
#define inited (__libtmrenderer_inited)

static bool isshadercompilationsuccessful(GLuint shader);
static void applytexlinearfilts(GLenum target);
static void onresize(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }
static void keycallback(GLFWwindow *window, int keycode, int scancode, int action, int mods)
{ if (ctxn.keycallback) ctxn.keycallback(keycode, action, mods); }
static void oninputchar(GLFWwindow *window, unsigned int codepoint) { if (ctxn.charcallback) ctxn.charcallback(codepoint); }

#define SETCTXUFIELDHELPER(fieldname, uniformstrname) (ctxu.fieldname = glGetUniformLocation(ctxn.prog, uniformstrname))

NError tmrenderer_init(const char *title)
{
    if (inited) return NError_AlreadyInitialized;
    if (!monotime_now(NULL)) goto errorquit_afterinitglfw;

    if (!glfwInit()) goto errorquit_generic;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!(ctxn.window = glfwCreateWindow(MINWINWIDTH, MINWINHEIGHT, title, NULL, NULL))) goto errorquit_afterinitglfw;
    glfwMakeContextCurrent(ctxn.window);

    if (!gladLoadGL(glfwGetProcAddress)) goto errorquit_afterinitglfw;

    onresize(ctxn.window, MINWINWIDTH, MINWINHEIGHT);
    glfwSetWindowSizeLimits(ctxn.window, MINWINWIDTH, MINWINHEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetFramebufferSizeCallback(ctxn.window, onresize);
    glfwSetInputMode(ctxn.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetKeyCallback(ctxn.window, keycallback);
    glfwSetCharCallback(ctxn.window, oninputchar);
    
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

        ctxn.prog = glCreateProgram();
        glAttachShader(ctxn.prog, vs);
        glAttachShader(ctxn.prog, fs);
        glLinkProgram(ctxn.prog);
        glUseProgram(ctxn.prog);

        GLint result;
        glGetProgramiv(ctxn.prog, GL_LINK_STATUS, &result);
        if (!result) goto errorquit_afterinitglfw;
    }

    SETCTXUFIELDHELPER(curblinkstate, "curblinkstate");
    SETCTXUFIELDHELPER(curenabled, "curenabled");
    SETCTXUFIELDHELPER(curpos, "curpos");
    SETCTXUFIELDHELPER(curbounds, "curbounds");
    SETCTXUFIELDHELPER(curuseshape, "curuseshape");
    SETCTXUFIELDHELPER(textblinkstate, "textblinkstate");
    SETCTXUFIELDHELPER(colors, "colors");

    // ==========================================================================================

    SETCURENABLED(true);
    SETCURPOS(0, 0);
    SETCURBOUNDS(14, 15);
    SETCURUSESHAPE(false);
    glUniform3fv(ctxu.colors, 16, CGAcolors);
    
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

    glGenTextures(1, &ctxn.vvmem);
    glUniform1i(glGetUniformLocation(ctxn.prog, "vmem"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ctxn.vvmem);
    applytexlinearfilts(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8UI, 80, 25, 0, GL_RG_INTEGER, GL_UNSIGNED_BYTE, NULL);

    glGenTextures(1, &ctxn.font);
    glUniform1i(glGetUniformLocation(ctxn.prog, "font"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ctxn.font);
    applytexlinearfilts(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, 16, 256, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
    
    glGenTextures(1, &ctxn.curshape);
    glUniform1i(glGetUniformLocation(ctxn.prog, "curshape"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_1D, ctxn.curshape);
    applytexlinearfilts(GL_TEXTURE_1D);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R8UI, 16, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);

    // ==========================================================================================

    v_curblinkperiod = DEFAULTCURBLINKPERIOD;
    v_textblinkperiod = DEFAULTTEXTBLINKPERIOD;

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
    memset(&ctxn, 0, sizeof(ctxn));
    memset(&ctxu, -1, sizeof(ctxu));

    inited = false;
    return NError_Success;
}

static bool isshadercompilationsuccessful(GLuint shader)
{
    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    return result;
}

static void applytexlinearfilts(GLenum target)
{
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    if (target != GL_TEXTURE_1D) glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}