#include "renderfuncs.h"

void vmem_clear(uint16_t *vmem, uint8_t attribute)
{
    register uint16_t offattr = attribute << 8;
    for (size_t i = 0; i < 2000; i++) vmem[i] = offattr;
}

void vmem_writeas(uint16_t *vmem, uint16_t offset, uint8_t attribute, const char *str)
{
    register char c;
    register uint16_t offattr = attribute << 8;
    register size_t i = 0;
    while ((c = str[i]) != 0) { vmem[offset + i] = offattr | c; i++; }
}

void vmem_writecs(uint16_t *vmem, uint16_t offset, const char *str)
{
    register char c;
    register size_t i = 0;
    while ((c = str[i]) != 0) { vmem[offset + i] = vmem[offset + i] & 0xFF00 | c; i++; }
}

void vmem_writeacs(uint16_t *vmem, uint16_t offset, const uint16_t *str)
{
    register uint16_t ac;
    register size_t i = 0;
    while ((ac = str[i]) & 0xFF != 0) { vmem[offset + i] = ac; i++; }
}