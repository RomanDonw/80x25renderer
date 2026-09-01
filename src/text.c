/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "context.h"
#include "init.h"
#include "constants.h"
#include "text.h"

struct textstate_s __libtmrenderer_textstate;

NError tmrenderer_getvramptr(uint16_t **vram)
{
    ENSURE_INIT;
    *vram = textstate.vram;
    return NError_Success;
}

NError tmrenderer_gettextblinkperiod(monotime_t *period)
{
    ENSURE_INIT;
    *period = textstate.blinkperiod;
    return NError_Success;
}

NError tmrenderer_settextblinkperiod(monotime_t period)
{
    ENSURE_INIT;
    textstate.blinkperiod = period ? period : DEFAULTTEXTBLINKPERIOD;
    return NError_Success;
}

NError tmrenderer_getkeystate(int keycode, int *state)
{
    ENSURE_INIT;
    *state = glfwGetKey(context.window, keycode);
    return NError_Success;
}

NError tmrenderer_getkeyname(int keycode, const char **keyname)
{
    ENSURE_INIT;
    *keyname = glfwGetKeyName(keycode, 0);
    return NError_Success;
}

NError tmrenderer_getkeycallback(TMRendererKeyInputCallback *callback)
{
    ENSURE_INIT;
    *callback = textstate.keycallback;
    return NError_Success;
}

NError tmrenderer_getcharcallback(TMRendererCharInputCallback *callback)
{
    ENSURE_INIT;
    *callback = textstate.charcallback;
    return NError_Success;
}

NError tmrenderer_setkeycallback(TMRendererKeyInputCallback callback)
{
    ENSURE_INIT;
    textstate.keycallback = callback;
    return NError_Success;
}

NError tmrenderer_setcharcallback(TMRendererCharInputCallback callback)
{
    ENSURE_INIT;
    textstate.charcallback = callback;
    return NError_Success;
}

NError tmrenderer_loadcolors(const float *colors)
{
    ENSURE_INIT;
    if (!colors) colors = CGAcolors;
    glUniform3fv(textstate.u_colors, 16, colors);
    return NError_Success;
}

NError tmrenderer_storecolors(float *colors)
{
    ENSURE_INIT;
    glGetUniformfv(context.prog, textstate.u_colors, colors);
    return NError_Success;
}

NError tmrenderer_loadfont(const uint8_t *font)
{
    ENSURE_INIT;
    glBindTexture(GL_TEXTURE_2D, textstate.tex_font);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16, 256, GL_RED_INTEGER, GL_UNSIGNED_BYTE, font);
    glBindTexture(GL_TEXTURE_2D, 0);
    return NError_Success;
}

NError tmrenderer_storefont(uint8_t *font)
{
    ENSURE_INIT;
    glBindTexture(GL_TEXTURE_2D, textstate.tex_font);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, font);
    glBindTexture(GL_TEXTURE_2D, 0);
    return NError_Success;
}