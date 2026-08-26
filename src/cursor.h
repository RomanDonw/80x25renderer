/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef CURSOR_H
#define CURSOR_H

#include <libmonotime.h>

extern bool __libtmrenderer_curenabled, __libtmrenderer_curuseshape;
extern unsigned char __libtmrenderer_curx, __libtmrenderer_cury, __libtmrenderer_curstartline, __libtmrenderer_curendline;
extern monotime_t __libtmrenderer_curblinkperiod;
#define v_curenabled (__libtmrenderer_curenabled)
#define v_curuseshape (__libtmrenderer_curuseshape)
#define v_curx (__libtmrenderer_curx)
#define v_cury (__libtmrenderer_cury)
#define v_curstartline (__libtmrenderer_curstartline)
#define v_curendline (__libtmrenderer_curendline)
#define v_curblinkperiod (__libtmrenderer_curblinkperiod)

#define SETCURENABLED(state) \
    v_curenabled = (state);\
    glUniform1ui(ctxu.curenabled, (state));

#define SETCURPOS(x, y) \
    v_curx = (x);\
    v_cury = (y);\
    glUniform2ui(ctxu.curpos, (x), (y));

#define SETCURBOUNDS(startline, endline) \
    v_curstartline = (startline);\
    v_curendline = (endline);\
    glUniform2ui(ctxu.curbounds, (startline), (endline));

#define SETCURUSESHAPE(state) \
    v_curuseshape = (state);\
    glUniform1ui(ctxu.curuseshape, (state));

#endif