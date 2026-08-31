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
#include <libmonotime.h>
#include <GLFW/glfw3.h>

// !!! FULL NOT THREAD-SAFE FOR NOW !!!

NError tmrenderer_init(const char *title);
NError tmrenderer_quit(void);

NError tmrenderer_getshouldclose(bool *state);
NError tmrenderer_setshouldclose(bool state);

//NError tmrenderer_refresh(const uint16_t *vvmem);
NError tmrenderer_flush(void);
NError tmrenderer_render(void);
NError tmrenderer_pollevents(bool wait);

NError tmrenderer_loadfont(const uint8_t *font);
NError tmrenderer_loadcurcustomshape(const uint8_t *shape);
NError tmrenderer_loadcolors(const float *colors); // colors can be NULL.

//NError tmrenderer_loadvvmem(const uint16_t *vvmem);
//NError tmrenderer_updatevvmem(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint16_t *data);

NError tmrenderer_getcurenabled(bool *state);
NError tmrenderer_setcurenabled(bool state);
NError tmrenderer_getcurpos(uint8_t *x, uint8_t *y);
NError tmrenderer_setcurpos(uint8_t x, uint8_t y);
NError tmrenderer_getcurbounds(uint8_t *startline, uint8_t *endline);
NError tmrenderer_setcurbounds(uint8_t startline, uint8_t endline);
NError tmrenderer_getcurusecustomshape(bool *state);
NError tmrenderer_setcurusecustomshape(bool state);

NError tmrenderer_gettextblinkperiod(monotime_t *period);
NError tmrenderer_settextblinkperiod(monotime_t period); // value 0 restores the default value.
NError tmrenderer_getcurblinkperiod(monotime_t *period);
NError tmrenderer_setcurblinkperiod(monotime_t period); // value 0 restores the default value.

// this functions uses GLFW key codes & states.

typedef void (*TMRendererKeyInputCallback)(int keycode, int action, int mods);
typedef void (*TMRendererCharInputCallback)(unsigned int codepoint);

NError tmrenderer_getkeystate(int keycode, int *state);
NError tmrenderer_getkeyname(int keycode, const char **keyname);
NError tmrenderer_getkeycallback(TMRendererKeyInputCallback *callback);
NError tmrenderer_setkeycallback(TMRendererKeyInputCallback callback);
NError tmrenderer_getcharcallback(TMRendererCharInputCallback *callback);
NError tmrenderer_setcharcallback(TMRendererCharInputCallback callback);

#endif