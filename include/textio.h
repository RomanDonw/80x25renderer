/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef TEXTIO_H
#define TEXTIO_H

#include <stddef.h>
#include <stdint.h>

void vmem_clear(uint16_t *vmem, uint8_t attribute);
void vmem_setattr(uint16_t *vmem, uint16_t offset, uint16_t count, uint8_t attribute);
void vmem_writeas(uint16_t *vmem, uint16_t offset, uint8_t attribute, const unsigned char *str);
void vmem_writecs(uint16_t *vmem, uint16_t offset, const unsigned char *str);
void vmem_writeacs(uint16_t *vmem, uint16_t offset, const uint16_t *str);

#endif
