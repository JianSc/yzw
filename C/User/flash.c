#include "flash.h"

int FLASH_DATA_TEM_SUM_READ()
{
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    u32 d;
    u8 i;
    for (i = 0; i < 4; i++)
    {
        d = d << 8;
        d = d | FLASH_ReadByte(FLASH_ADDRES + i);
    }
    if (d == 0)
    {
        d = 0x0000012c;
        FLASH_ProgramWord(FLASH_ADDRES, d);
    }
    u16 d1, d2;
    d1 = d >> 16;
    d2 = d;
    int d3 = d2;
    if (d1 == 0xffff)
    {
        d3 = -d3;
    }
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    delay_ms(10);
    return d3;
}
bool FLASH_DATA_LED_STATUS_READ()
{
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    bool b;
    u8 d = FLASH_ReadByte(FLASH_ADDRES + 4);
    b = (d == 0xf1) ? TRUE : FALSE;
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    delay_ms(10);
    return b;
}
bool FLASH_DATA_TEM_STATUS_READ()
{
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    bool b;
    u8 d = FLASH_ReadByte(FLASH_ADDRES + 5);
    b = (d == 0xf1) ? TRUE : FALSE;
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    delay_ms(10);
    return b;
}
u8 FLASH_DATA_FAN_PWM_READ()
{
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    u8 d = FLASH_ReadByte(FLASH_ADDRES + 6);
    if (d == 0)
    {
        d = 60;
        FLASH_ProgramByte(FLASH_ADDRES + 6, d);
    }
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    delay_ms(10);
    return d;
}

bool FLASH_DATA_HF_STATUS_READ()
{
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    bool b;
    u8 d = FLASH_ReadByte(FLASH_ADDRES + 7);
    b = (d == 0xf1) ? TRUE : FALSE;
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    delay_ms(10);
    return b;
}

void FLASH_DATA_HF_STATUS_WRITE(bool b)
{
    u8 d;
    d = b ? 0xf1 : 0x00;
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    FLASH_ProgramByte(FLASH_ADDRES + 7, d);
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    delay_ms(10);
}

void FLASH_DATA_TEM_SUM_WRITE(int d)
{
    u16 d1, d2;
    u32 d3;
    if (d < 0)
    {
        d1 = 0xffff;
    }
    else
    {
        d1 = 0x0000;
    }
    d2 = d;
    d3 = d1;
    d3 = d3 << 16;
    d3 = d3 | d2;
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    FLASH_ProgramWord(FLASH_ADDRES, d3);
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    delay_ms(10);
}
void FLASH_DATA_LED_STATUS_WRITE(bool b)
{
    u8 d;
    d = b ? 0xf1 : 0x00;
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    FLASH_ProgramByte(FLASH_ADDRES + 4, d);
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    delay_ms(10);
}
void FLASH_DATA_TEM_STATUS_WRTIE(bool b)
{
    u8 d;
    d = b ? 0xf1 : 0x00;
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    FLASH_ProgramByte(FLASH_ADDRES + 5, d);
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    delay_ms(10);
}
void FLASH_DATA_FAN_PWM_WRITE(u8 d)
{
    u8 d1 = (d < 50) ? 50 : d;
    d1 = (d1 > 100) ? 100 : d1;
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    FLASH_ProgramByte(FLASH_ADDRES + 6, d1);
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    delay_ms(10);
}