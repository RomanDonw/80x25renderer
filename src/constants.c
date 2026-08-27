/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "constants.h"

const float __libtmrenderer_CGAcolors[] =
{
    0, 0, 0, // black.
    0, 0, 0xAA / 255.0, // dark blue.
    0, 0xAA / 255.0, 0, // dark green.
    0, 0xAA / 255.0, 0xAA / 255.0, // dark cyan.
    0xAA / 255.0, 0, 0, // dark red.
    0xAA / 255.0, 0, 0xAA / 255.0, // dark magenta.
    0xAA / 255.0, 0x55 / 255.0, 0, // dark yellow/brown.
    0xAA / 255.0, 0xAA / 255.0, 0xAA / 255.0, // gray.

    0x55 / 255.0, 0x55 / 255.0, 0x55 / 255.0, // dark gray.
    0x55 / 255.0, 0x55 / 255.0, 1, // light blue.
    0x55 / 255.0, 1, 0x55 / 255.0, // light green.
    0x55 / 255.0, 1, 1, // light cyan.
    1, 0x55 / 255.0, 0x55 / 255.0, // light red.
    1, 0x55 / 255.0, 1, // light magenta.
    1, 1, 0x55 / 255.0, // light yellow.
    1, 1, 1 // light white.
};