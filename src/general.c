/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "context.h"
#include "init.h"
#include "cursor.h"
#include "tmrenderer.h"
#include "text.h"
#include "constants.h"

NError tmrenderer_getshouldclose(bool *state)
{
    ENSURE_INIT;
    *state = glfwWindowShouldClose(context.window);
    return NError_Success;
}

NError tmrenderer_setshouldclose(bool state)
{
    ENSURE_INIT;
    glfwSetWindowShouldClose(context.window, state);
    return NError_Success;
}

NError tmrenderer_render(void)
{
    ENSURE_INIT;

    monotime_t currtime;
    if (!monotime_now(&currtime)) return NError_Fault;
    glUniform1ui(context.u_curblinkstate, (bool)((currtime / cursorcachedstate.blinkperiod) % 2));
    glUniform1ui(context.u_textblinkstate, (bool)((currtime / textstate.blinkperiod) % 2));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    glfwSwapBuffers(context.window);
    return NError_Success;
}

NError tmrenderer_pollevents(bool wait)
{
    ENSURE_INIT;
    wait ? glfwWaitEvents() : glfwPollEvents();
    return NError_Success;
}

NError tmrenderer_loadfont(const uint8_t *font)
{
    ENSURE_INIT;
    glBindTexture(GL_TEXTURE_2D, context.font);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16, 256, GL_RED_INTEGER, GL_UNSIGNED_BYTE, font);
    glBindTexture(GL_TEXTURE_2D, 0);
    return NError_Success;
}

NError tmrenderer_loadvvmem(const uint16_t *vvmem)
{
    ENSURE_INIT;
    glBindTexture(GL_TEXTURE_2D, context.vvmem);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 80, 25, GL_RG_INTEGER, GL_UNSIGNED_BYTE, vvmem);
    glBindTexture(GL_TEXTURE_2D, 0);
    return NError_Success;
}

NError tmrenderer_updatevvmem(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint16_t *data)
{
    ENSURE_INIT;
    NError nerr = NError_Success;
    glBindTexture(GL_TEXTURE_2D, context.vvmem);

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RG_INTEGER, GL_UNSIGNED_BYTE, data);
    if (glGetError()) nerr = NError_Fault;

    glBindTexture(GL_TEXTURE_2D, 0);
    return nerr;
}

NError tmrenderer_loadcurcustomshape(const uint8_t *shape)
{
    ENSURE_INIT;
    glBindTexture(GL_TEXTURE_1D, context.curcustomshape);
    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 16, GL_RED_INTEGER, GL_UNSIGNED_BYTE, shape);
    glBindTexture(GL_TEXTURE_1D, 0);
    return NError_Success;
}

NError tmrenderer_loadcolors(const float *colors)
{
    ENSURE_INIT;
    if (!colors) colors = CGAcolors;
    glUniform3fv(context.u_colors, 16, colors);
    return NError_Success;
}