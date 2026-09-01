/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "tmrenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool clrnext = false;
static uint8_t attr = 7;
static uint16_t *vram;

static inline void clrscr(uint16_t v)
{ for (uint16_t i = 0; i < 2000; i++) vram[i] = v; }

static void charcallback(unsigned int codepoint)
{
    unsigned char x, y;
    tmrenderer_getcurpos(&x, &y);
    if (codepoint > 31 && codepoint < 127 || codepoint >= 0x410 && codepoint <= 0x44F || codepoint == 0x401 || codepoint == 0x451)
    {
        if (y >= 24 && x >= 79) return;
        
        uint16_t chr = attr << 8;
        if (codepoint == 0x451) chr |= 0xF1;
        else if (codepoint == 0x401) chr |= 0xF0;
        else if (codepoint < 127) chr |= codepoint;
        else if (codepoint < 0x440) chr |= codepoint - 0x410 + 0x80;
        else chr |= codepoint - 0x440 + 0xE0;
        
        if (clrnext)
        {
            clrscr(chr);
            clrnext = false;
        }
        else
        {
            vram[y * 80 + x] = chr;
            if (++x > 79) { x = 0; y++; }
            tmrenderer_setcurpos(x, y);
        }
    }
}

static void keycallback(int key, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        unsigned char x, y;
        tmrenderer_getcurpos(&x, &y);
        if (action == GLFW_PRESS)
        {
            /*
            if (mods & GLFW_MOD_CONTROL && key == GLFW_KEY_B)
            {
                if (mods & GLFW_MOD_SHIFT) (use reading video memory segment here).
                else attr ^= 0x80;
            }
            */
            if (mods & GLFW_MOD_CONTROL)
            {
                if (mods & GLFW_MOD_SHIFT)
                {
                    if (key == GLFW_KEY_0) attr = attr & 0x8F;
                    else if (key == GLFW_KEY_1) attr = attr & 0x8F | 0x10;
                    else if (key == GLFW_KEY_2) attr = attr & 0x8F | 0x20;
                    else if (key == GLFW_KEY_3) attr = attr & 0x8F | 0x30;
                    else if (key == GLFW_KEY_4) attr = attr & 0x8F | 0x40;
                    else if (key == GLFW_KEY_5) attr = attr & 0x8F | 0x50;
                    else if (key == GLFW_KEY_6) attr = attr & 0x8F | 0x60;
                    else if (key == GLFW_KEY_7) attr = attr & 0x8F | 0x70;
                }
                else
                {
                    if (key == GLFW_KEY_B) attr ^= 0x80;
                    else if (key == GLFW_KEY_C) clrnext = true;
                    else if (key == GLFW_KEY_0) attr = attr & 0xF0;
                    else if (key == GLFW_KEY_1) attr = attr & 0xF0 | 1;
                    else if (key == GLFW_KEY_2) attr = attr & 0xF0 | 2;
                    else if (key == GLFW_KEY_3) attr = attr & 0xF0 | 3;
                    else if (key == GLFW_KEY_4) attr = attr & 0xF0 | 4;
                    else if (key == GLFW_KEY_5) attr = attr & 0xF0 | 5;
                    else if (key == GLFW_KEY_6) attr = attr & 0xF0 | 6;
                    else if (key == GLFW_KEY_7) attr = attr & 0xF0 | 7;
                    else if (key == GLFW_KEY_8) attr = attr & 0xF0 | 8;
                    else if (key == GLFW_KEY_9) attr = attr & 0xF0 | 9;
                    else if (key == GLFW_KEY_A) attr = attr & 0xF0 | 10;
                    else if (key == GLFW_KEY_B) attr = attr & 0xF0 | 11;
                    else if (key == GLFW_KEY_C) attr = attr & 0xF0 | 12;
                    else if (key == GLFW_KEY_D) attr = attr & 0xF0 | 13;
                    else if (key == GLFW_KEY_E) attr = attr & 0xF0 | 14;
                    else if (key == GLFW_KEY_F) attr = attr & 0xF0 | 15;
                }
                return;
            }
        }
 
        if (key == GLFW_KEY_BACKSPACE)
        {
            if (!(x || y)) return;
            if (!x) { y--; x = 79; }
            else x--;
            
            vram[y * 80 + x] = attr << 8;
            tmrenderer_setcurpos(x, y);
        }
        else if (key == GLFW_KEY_ENTER)
        {
            if (y++ >= 24) return;
            x = 0;
            tmrenderer_setcurpos(x, y);
        }
        else if (key == GLFW_KEY_UP)
        {
            if (!(y--)) return;
            tmrenderer_setcurpos(x, y);
        }
        else if (key == GLFW_KEY_DOWN)
        {
            if (y++ >= 24) return;
            tmrenderer_setcurpos(x, y);
        }
        else if (key == GLFW_KEY_LEFT)
        {
            if (!(x--)) return;
            tmrenderer_setcurpos(x, y);
        }
        else if (key == GLFW_KEY_RIGHT)
        {
            if ((x++) >= 79) return;
            tmrenderer_setcurpos(x, y);
        }
        else if (key == GLFW_KEY_DELETE)
        {
            clrscr(attr << 8);
            clrnext = false;
        }
        else if (key == GLFW_KEY_HOME) tmrenderer_setcurpos(0, y);
        else if (key == GLFW_KEY_END) tmrenderer_setcurpos(79, y);
    }
}

int main(void)
{
    if (tmrenderer_init("test") != NError_Success) { puts("failed to initialize renderer"); return 1; }

    
    {
        uint8_t *fontdata = malloc(4096);
        if (!fontdata) { puts("memory allocation failed"); return 1; }

        FILE *f = fopen("res/cp866.f16", "r");
        if (!f) { puts("unable to load font from file \"res/cp866.f16\""); return 1; }
        
        if (!fread(fontdata, 4096, 1, f) || feof(f) || ferror(f))
        { puts("error reading font from file \"res/cp866.f16\""); return 1; }

        fclose(f);
        tmrenderer_loadfont((void *)fontdata);
        free(fontdata);
    }

    if (tmrenderer_getvramptr(&vram) != NError_Success) { puts("unable to get pointer to VRAM."); return 1; }

    clrscr(attr << 8);
    tmrenderer_setcharcallback(charcallback);
    tmrenderer_setkeycallback(keycallback);

    bool shouldclose;
    while (tmrenderer_getshouldclose(&shouldclose) == NError_Success && !shouldclose)
    {
        //(*((uint16_t *)buff))++;
        //tmrenderer_updatevvmem(0, 0, 1, 1, buff);
        
        if (
            (tmrenderer_flush() != NError_Success) ||
            (tmrenderer_render() != NError_Success) ||
            (tmrenderer_pollevents(false) != NError_Success)
        ) break;
    }
    
    tmrenderer_quit();
    return 0;
}