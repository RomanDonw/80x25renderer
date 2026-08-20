#include "tmrenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    if (tmrenderer_init("test") != NError_Success) { puts("failed to initialize renderer"); return 1; }

    void *buff = malloc(4096);

    {
        FILE *f = fopen("res/cp866.f16", "r");
        if (!f) { puts("unable to load font from file \"res/cp866.f16\""); return 1; }
        fread(buff, 1, 4096, f);
        fclose(f);
        tmrenderer_loadfont(buff);
    }

    memset(buff, 0x47, 4000);
    tmrenderer_loadvvmem(buff);

    do
    {
        (*((uint16_t *)buff))++;
        tmrenderer_loadvvmem(buff);
    } while (tmrenderer_render() == NError_Success);

    free(buff);
    tmrenderer_quit();
    return 0;
}