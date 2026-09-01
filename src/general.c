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

struct context_s __libtmrenderer_context;

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

NError tmrenderer_flush(void)
{
    ENSURE_INIT;

    glBindTexture(GL_TEXTURE_2D, textstate.tex_vram);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 80, 25, GL_RG_INTEGER, GL_UNSIGNED_BYTE, textstate.vram);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glUniform2ui(cursorstate.u_bounds, cursorstate.startline, cursorstate.endline);
    glUniform1ui(cursorstate.u_enabled, cursorstate.enabled);
    glUniform2ui(cursorstate.u_pos, cursorstate.x, cursorstate.y);
    glUniform1ui(cursorstate.u_usecustomshape, cursorstate.usecustomshape);
    
    return NError_Success;
}

NError tmrenderer_render(void)
{
    ENSURE_INIT;

    monotime_t currtime;
    if (!monotime_now(&currtime)) return NError_Fault;
    glUniform1ui(cursorstate.u_blinkstate, (bool)((currtime / cursorstate.blinkperiod) % 2));
    glUniform1ui(textstate.u_blinkstate, (bool)((currtime / textstate.blinkperiod) % 2));

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