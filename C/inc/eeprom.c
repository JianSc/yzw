#include "eeprom.h"

u16 eeprom_read16(u16 address, u16 x)
{
    u16 d;
    u8 *p;
    p = (u8 *)(address);
    d = *p << 8;
    p = (u8 *)(address + 1);
    d |= *p;

    if (d == 0 && x != 0) {
        d = x;
        disableInterrupts();
        do {
            FLASH_DUKR = 0xae;
            FLASH_DUKR = 0x56;
        } while (!FLASH_IAPSR_DUL);
        p  = (u8 *)(address);
        *p = d >> 8;
        p  = (u8 *)(address + 1);
        *p = d;
        while (!FLASH_IAPSR_EOP) {}
        FLASH_IAPSR_DUL = 0;
        enableInterrupts();
    }
    return d;
}
u8 eeprom_read(u16 address, u8 x)
{
    u8 d, *p;
    p = (u8 *)(address);
    d = *p;
    if (d == 0 && x != 0) {
        d = x;
        disableInterrupts();
        do {
            FLASH_DUKR = 0xae;
            FLASH_DUKR = 0x56;
        } while (!FLASH_IAPSR_DUL);
        *p = d;
        while (!FLASH_IAPSR_EOP) {}
        FLASH_IAPSR_DUL = 0;
        enableInterrupts();
    }
    return d;
}
void eeprom_write16(u16 address, u16 data)
{
    u8 *p;
    disableInterrupts();
    do {
        FLASH_DUKR = 0xae;
        FLASH_DUKR = 0x56;
    } while (!FLASH_IAPSR_DUL);
    p  = (u8 *)(address);
    *p = data >> 8;
    p  = (u8 *)(address + 1);
    *p = data;
    while (!FLASH_IAPSR_EOP) {}
    FLASH_IAPSR_DUL = 0;
    enableInterrupts();
}
void eeprom_write(u16 address, u8 data)
{
    u8 *p;
    disableInterrupts();
    do {
        FLASH_DUKR = 0xae;
        FLASH_DUKR = 0x56;
    } while (!FLASH_IAPSR_DUL);
    p  = (u8 *)(address);
    *p = data;
    while (!FLASH_IAPSR_EOP) {}
    FLASH_IAPSR_DUL = 0;
    enableInterrupts();
}