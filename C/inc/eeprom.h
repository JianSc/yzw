#ifndef _EEPROM_H
#define _EEPROM_H

#include "init.h"

u16 eeprom_read16(u16 address, u16 x);
u8 eeprom_read(u16 address, u8 x);
void eeprom_write16(u16 address, u16 data);
void eeprom_write(u16 address, u8 data);

#endif