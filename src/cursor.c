/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "cursor.h"

#include "init.h"
#include "tmrenderer.h"
#include "constants.h"

bool __libtmrenderer_curenabled, __libtmrenderer_curuseshape;
unsigned char __libtmrenderer_curx, __libtmrenderer_cury, __libtmrenderer_curstartline, __libtmrenderer_curendline;
monotime_t __libtmrenderer_curblinkperiod;

NError tmrenderer_getcurenabled(bool *state)
{
    ENSURE_INIT;
    *state = v_curenabled;
    return NError_Success;
}

NError tmrenderer_setcurenabled(bool state)
{
    ENSURE_INIT;
    SETCURENABLED(state);
    return NError_Success;
}

NError tmrenderer_getcurpos(unsigned char *x, unsigned char *y)
{
    ENSURE_INIT;
    *x = v_curx;
    *y = v_cury;
    return NError_Success;
}

NError tmrenderer_setcurpos(unsigned char x, unsigned char y)
{
    ENSURE_INIT;
    SETCURPOS(x, y);
    return NError_Success;
}

NError tmrenderer_getcurbounds(unsigned char *startline, unsigned char *endline)
{
    ENSURE_INIT;
    *startline = v_curstartline;
    *endline = v_curendline;
    return NError_Success;
}

NError tmrenderer_setcurbounds(unsigned char startline, unsigned char endline)
{
    ENSURE_INIT;
    SETCURBOUNDS(startline, endline);
    return NError_Success;
}

NError tmrenderer_getcuruseshape(bool *state)
{
    ENSURE_INIT;
    *state = v_curuseshape;
    return NError_Success;
}

NError tmrenderer_setcuruseshape(bool state)
{
    ENSURE_INIT;
    SETCURUSESHAPE(state);
    return NError_Success;
}

NError tmrenderer_getcurblinkperiod(monotime_t *period)
{
    ENSURE_INIT;
    *period = v_curblinkperiod;
    return NError_Success;
}

NError tmrenderer_setcurblinkperiod(monotime_t period)
{
    ENSURE_INIT;
    v_curblinkperiod = period ? period : DEFAULTCURBLINKPERIOD;
    return NError_Success;
}