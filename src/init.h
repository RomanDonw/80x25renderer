/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef INIT_H
#define INIT_H

#include <libncore.h>
#include <stdbool.h>
#include <glad.h>
#include <GLFW/glfw3.h>

#ifdef INIT_C
    #define INIT_H_EXTERN_QUALIFIER
#else
    #define INIT_H_EXTERN_QUALIFIER const
#endif

extern INIT_H_EXTERN_QUALIFIER bool __libtmrenderer_inited;
#define ENSURE_INIT { if (!__libtmrenderer_inited) return NError_NotInitialized; }

// nullable context vars.
struct ctxn_s
{
    GLFWwindow *window;
    GLuint font, vvmem, curshape, prog;
};
extern INIT_H_EXTERN_QUALIFIER struct ctxn_s __libtmrenderer_ctxn;
#define ctxn (__libtmrenderer_ctxn)

// uniform vars.
struct ctxu_s
{ GLint colors, textblinkstate, curenabled, curpos, curbounds, curblinkstate, curuseshape; }
extern INIT_H_EXTERN_QUALIFIER __libtmrenderere_ctxu;
#define ctxu (__libtmrenderere_ctxu)

extern monotime_t __libtmrenderer_textblinkperiod, __libtmrenderer_curblinkperiod;
#define textblinkperiod (__libtmrenderer_textblinkperiod)
#define curblinkperiod (__libtmrenderer_curblinkperiod)

#endif