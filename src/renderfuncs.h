#ifndef RENDERFUNCS_H
#define RENDERFUNCS_H

#include <stddef.h>
#include <stdint.h>

void vmem_clear(uint16_t *vmem, uint8_t attribute);
void vmem_writeas(uint16_t *vmem, uint16_t offset, uint8_t attribute, const char *str);
void vmem_writecs(uint16_t *vmem, uint16_t offset, const char *str);

#endif