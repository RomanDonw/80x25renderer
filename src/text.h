/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef TEXT_H
#define TEXT_H

#include <libmonotime.h>
#include <stdint.h>
#include "tmrenderer.h"

#define DEFAULTTEXTBLINKPERIOD 533333333

struct textstate_s
{
    uint16_t *ram_vvmem;
    monotime_t blinkperiod;
    TMRendererKeyInputCallback keycallback;
    TMRendererCharInputCallback charcallback;

    GLuint tex_vvmem;
    GLint u_textblinkstate, u_colors;
};
extern struct textstate_s __libtmrenderer_textstate;
#define textstate (__libtmrenderer_textstate)

#endif