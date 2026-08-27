/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef INIT_H
#define INIT_H

#include <libncore.h>

#ifdef INIT_C
    #define INIT_H_EXTERN_QUALIFIER
#else
    #define INIT_H_EXTERN_QUALIFIER const
#endif

extern INIT_H_EXTERN_QUALIFIER bool __libtmrenderer_inited;
#define ENSURE_INIT if (!__libtmrenderer_inited) return NError_NotInitialized;

#endif