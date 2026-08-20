/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef TEXTMODERENDERER_H
#define TEXTMODERENDERER_H

#include <libncore.h>
#include <stdbool.h>
#include <stdint.h>

// !!! FULL NOT THREAD-SAFE FOR NOW !!!

NError tmrenderer_init(const char *title);
NError tmrenderer_quit(void);

//NError tmrenderer_refresh(const uint16_t *vvmem);
NError tmrenderer_render(void); // returns NError_Interrupted when user wants to close renderer`s window.

NError tmrenderer_loadfont(const unsigned char *font);
NError tmrenderer_loadvvmem(const uint16_t *vvmem);
NError tmrenderer_loadcurshape(const unsigned char *curshape);
NError tmrenderer_loadcolors(const float *colors); // colors can be NULL.

NError tmrenderer_curenabled(bool state);
NError tmrenderer_curpos(unsigned char x, unsigned char y);
NError tmrenderer_curbounds(unsigned char startline, unsigned char endline);
NError tmrenderer_curuseshape(bool state);

#endif