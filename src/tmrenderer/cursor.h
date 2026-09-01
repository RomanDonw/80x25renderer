/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef CURSOR_H
#define CURSOR_H

#include <stdbool.h>
#include <stdint.h>
#include <libmonotime.h>

#include "context.h"

#define DEFAULTCURBLINKPERIOD 266666667

struct cursorstate_s
{
    bool enabled, usecustomshape;
    uint8_t x, y, startline, endline;
    monotime_t blinkperiod;

    GLuint tex_customshape;
    GLint u_enabled, u_pos, u_bounds, u_blinkstate, u_usecustomshape;
};

extern struct cursorstate_s __libtmrenderer_cursorstate;
#define cursorstate (__libtmrenderer_cursorstate)

#define SETCURENABLED(state) \
    cursorstate.enabled = (state);\

#define SETCURPOS(_x, _y) \
    cursorstate.x = (_x);\
    cursorstate.y = (_y);

#define SETCURBOUNDS(_startline, _endline) \
    cursorstate.startline = (_startline);\
    cursorstate.endline = (_endline);

#define SETCURUSECUSTOMSHAPE(state) \
    cursorstate.usecustomshape = (state);

#endif