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

struct cursorcachedstate_s
{
    bool enabled, usecustomshape;
    uint8_t x, y, startline, endline;
    monotime_t blinkperiod;

    GLuint tex_customshape;
    GLint u_enabled, u_pos, u_bounds, u_blinkstate, u_usecustomshape;
};
extern struct cursorcachedstate_s __libtmrenderer_cursorcachedstate;
#define cursorcachedstate (__libtmrenderer_cursorcachedstate)

#define SETCURENABLED(state) \
    cursorcachedstate.enabled = (state);\
    glUniform1ui(cursorcachedstate.u_curenabled, (bool)(state));

#define SETCURPOS(_x, _y) \
    cursorcachedstate.x = (_x);\
    cursorcachedstate.y = (_y);\
    glUniform2ui(cursorcachedstate.u_curpos, (uint8_t)(_x), (uint8_t)(_y));

#define SETCURBOUNDS(_startline, _endline) \
    cursorcachedstate.startline = (_startline);\
    cursorcachedstate.endline = (_endline);\
    glUniform2ui(cursorcachedstate.u_curbounds, (uint8_t)(_startline), (uint8_t)(_endline));

#define SETCURUSECUSTOMSHAPE(state) \
    cursorcachedstate.usecustomshape = (state);\
    glUniform1ui(cursorcachedstate.u_curusecustomshape, (bool)(state));

#endif