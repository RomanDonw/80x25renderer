/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "tmrenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    if (tmrenderer_init("test") != NError_Success) { puts("failed to initialize renderer"); return 1; }

    void *buff = malloc(4096);
    if (!buff) { puts("memory allocation failed"); return 1; }

    {
        FILE *f = fopen("res/cp866.f16", "r");
        if (!f) { puts("unable to load font from file \"res/cp866.f16\""); return 1; }
        fread(buff, 1, 4096, f);
        fclose(f);
        tmrenderer_loadfont(buff);
    }

    memset(buff, 0x47, 4000);
    tmrenderer_loadvvmem(buff);

    bool shouldclose;
    while (tmrenderer_getshouldclose(&shouldclose) == NError_Success && !shouldclose)
    {
        (*((uint16_t *)buff))++;
        tmrenderer_updatevvmem(0, 0, 1, 1, buff);

        if (tmrenderer_render() != NError_Success) break;
        tmrenderer_pollevents(false);
    }

    free(buff);
    tmrenderer_quit();
    return 0;
}