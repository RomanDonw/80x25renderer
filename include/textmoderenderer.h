/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef TEXTMODERENDERER_H
#define TEXTMODERENDERER_H

#include <libncore.h>
#include <stdbool.h>

NError tmrenderer_init(int winwidth, int winheight);
NError tmrenderer_quit(void);

#endif