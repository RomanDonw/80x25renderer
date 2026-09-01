/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "cursor.h"

#include "init.h"
#include "tmrenderer.h"

struct cursorstate_s __libtmrenderer_cursorstate;

NError tmrenderer_getcurenabled(bool *state)
{
    ENSURE_INIT;
    *state = cursorstate.enabled;
    return NError_Success;
}

NError tmrenderer_setcurenabled(bool state)
{
    ENSURE_INIT;
    SETCURENABLED(state);
    return NError_Success;
}

NError tmrenderer_getcurpos(uint8_t *x, uint8_t *y)
{
    ENSURE_INIT;
    *x = cursorstate.x;
    *y = cursorstate.y;
    return NError_Success;
}

NError tmrenderer_setcurpos(uint8_t x, uint8_t y)
{
    ENSURE_INIT;
    SETCURPOS(x, y);
    return NError_Success;
}

NError tmrenderer_getcurbounds(uint8_t *startline, uint8_t *endline)
{
    ENSURE_INIT;
    *startline = cursorstate.startline;
    *endline = cursorstate.endline;
    return NError_Success;
}

NError tmrenderer_setcurbounds(uint8_t startline, uint8_t endline)
{
    ENSURE_INIT;
    SETCURBOUNDS(startline, endline);
    return NError_Success;
}

NError tmrenderer_getcurusecustomshape(bool *state)
{
    ENSURE_INIT;
    *state = cursorstate.usecustomshape;
    return NError_Success;
}

NError tmrenderer_setcurusecustomshape(bool state)
{
    ENSURE_INIT;
    SETCURUSECUSTOMSHAPE(state);
    return NError_Success;
}

NError tmrenderer_getcurblinkperiod(monotime_t *period)
{
    ENSURE_INIT;
    *period = cursorstate.blinkperiod;
    return NError_Success;
}

NError tmrenderer_setcurblinkperiod(monotime_t period)
{
    ENSURE_INIT;
    cursorstate.blinkperiod = period ? period : DEFAULTCURBLINKPERIOD;
    return NError_Success;
}

NError tmrenderer_loadcurcustomshape(const uint8_t *shape)
{
    ENSURE_INIT;
    glBindTexture(GL_TEXTURE_1D, cursorstate.tex_customshape);
    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 16, GL_RED_INTEGER, GL_UNSIGNED_BYTE, shape);
    glBindTexture(GL_TEXTURE_1D, 0);
    return NError_Success;
}

NError tmrenderer_storecurcustomshape(uint8_t *shape)
{
    ENSURE_INIT;
    glBindTexture(GL_TEXTURE_1D, cursorstate.tex_customshape);
    glGetTexImage(GL_TEXTURE_1D, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, shape);
    glBindTexture(GL_TEXTURE_1D, 0);
    return NError_Success;
}