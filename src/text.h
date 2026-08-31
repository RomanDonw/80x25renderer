/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef TEXT_H
#define TEXT_H

#include <libmonotime.h>

#define DEFAULTTEXTBLINKPERIOD 533333333

struct textstate_s
{
    monotime_t blinkperiod;
};
extern struct textstate_s __libtmrenderer_textstate;
#define textstate (__libtmrenderer_textstate)

#endif