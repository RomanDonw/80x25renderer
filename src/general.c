#include "tmrenderer.h"

#include <libmonotime.h>

#include "init.h"
#include "util.h"

NError tmrenderer_getshouldclose(bool *state)
{
    ENSURE_INIT;
    *state = glfwWindowShouldClose(ctxn.window);
    return NError_Success;
}

NError tmrenderer_setshouldclose(bool state)
{
    ENSURE_INIT;
    glfwSetWindowShouldClose(ctxn.window, state);
    return NError_Success;
}

NError tmrenderer_render(void)
{
    ENSURE_INIT;

    monotime_t currtime;
    if (!monotime_now(&currtime)) return NError_Fault;
    glUniform1ui(ctxu.curblinkstate, (bool)((currtime / 266666667) % 2));
    glUniform1ui(ctxu.textblinkstate, (bool)((currtime / 533333333) % 2));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    glfwSwapBuffers(ctxn.window);
    glfwPollEvents();
    return NError_Success;
}

NError tmrenderer_loadfont(const unsigned char *font)
{
    ENSURE_INIT;
    glBindTexture(GL_TEXTURE_2D, ctxn.font);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16, 256, GL_RED_INTEGER, GL_UNSIGNED_BYTE, font);
    glBindTexture(GL_TEXTURE_2D, 0);
    return NError_Success;
}

NError tmrenderer_loadvvmem(const uint16_t *vvmem)
{
    ENSURE_INIT;
    glBindTexture(GL_TEXTURE_2D, ctxn.vvmem);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 80, 25, GL_RG_INTEGER, GL_UNSIGNED_BYTE, vvmem);
    glBindTexture(GL_TEXTURE_2D, 0);
    return NError_Success;
}

NError tmrenderer_updatevvmem(unsigned char x, unsigned char y, unsigned char w, unsigned char h, const uint16_t *data)
{
    ENSURE_INIT;
    NError nerr = NError_Success;
    glBindTexture(GL_TEXTURE_2D, ctxn.vvmem);

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RG_INTEGER, GL_UNSIGNED_BYTE, data);
    if (glGetError()) nerr = NError_Fault;

    glBindTexture(GL_TEXTURE_2D, 0);
    return nerr;
}

NError tmrenderer_loadcurshape(const unsigned char *curshape)
{
    ENSURE_INIT;
    glBindTexture(GL_TEXTURE_1D, ctxn.curshape);
    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 16, GL_RED_INTEGER, GL_UNSIGNED_BYTE, curshape);
    glBindTexture(GL_TEXTURE_1D, 0);
    return NError_Success;
}

NError tmrenderer_loadcolors(const float *colors)
{
    ENSURE_INIT;
    if (!colors) colors = CGAcolors;
    glUniform3fv(ctxu.colors, 16, colors);
    return NError_Success;
}

NError tmrenderer_curenabled(bool state)
{
    ENSURE_INIT;
    glUniform1ui(ctxu.curenabled, state);
    return NError_Success;
}

NError tmrenderer_curpos(unsigned char x, unsigned char y)
{
    ENSURE_INIT;
    glUniform2ui(ctxu.curpos, x, y);
    return NError_Success;
}

NError tmrenderer_curbounds(unsigned char startline, unsigned char endline)
{
    ENSURE_INIT;
    glUniform2ui(ctxu.curbounds, startline, endline);
    return NError_Success;
}

NError tmrenderer_curuseshape(bool state)
{
    ENSURE_INIT;
    glUniform1ui(ctxu.curuseshape, state);
    return NError_Success;
}