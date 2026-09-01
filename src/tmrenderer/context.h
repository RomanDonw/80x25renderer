/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef CONTEXT_H
#define CONTEXT_H

#include <glad.h>
#include "tmrenderer.h"

struct context_s
{
    GLFWwindow *window;
    GLuint prog;
};
extern struct context_s __libtmrenderer_context;
#define context (__libtmrenderer_context)

#endif