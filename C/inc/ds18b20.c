#include "ds18b20.h"

bool ds18b20_init()
{
    u8 retry = 0;
    DS18B20_IO_OUT;
    DS18B20_DQ = 0;
    delay_us(600);
    DS18B20_DQ = 1;
    delay_us(10);
    DS18B20_IO_IN;
    while (DS18B20_DQ_READ && retry < 200) {
        retry++;
        delay_us(1);
    }
    if (retry >= 200) {
        return FALSE;
    } else {
        retry = 0;
    }
    while (!DS18B20_DQ_READ && retry < 240) {
        retry++;
        delay_us(1);
    }
    if (retry >= 240) {
        return FALSE;
    } else {
        return TRUE;
    }
}

u8 ds18b20_r()
{
    u8 d, i;
    for (i = 0; i < 8; i++) {
        d >>= 1;
        DS18B20_IO_OUT;
        DS18B20_DQ = 0;
        delay_us(2);
        DS18B20_DQ = 1;
        DS18B20_IO_IN;
        delay_us(12);
        if (DS18B20_DQ_READ) {
            d |= 0x80;
        }
        delay_us(50);
    }
    return d;
}

void ds18b20_w(u8 d)
{
    u8 i;
    DS18B20_IO_OUT;
    for (i = 0; i < 8; i++) {
        bool d2 = (d & 0x01) ? TRUE : FALSE;
        if (d2) {
            DS18B20_DQ = 0;
            delay_us(2);
            DS18B20_DQ = 1;
            delay_us(60);
        } else {
            DS18B20_DQ = 0;
            delay_us(60);
            DS18B20_DQ = 1;
            delay_us(2);
        }
        d >>= 1;
    }
}
u16 ds18b20_read()
{
    u16 dat;
    u8 th, tl;

    ds18b20_w(SKIP_ROM);
    ds18b20_w(READ_RAM);

    tl  = ds18b20_r();
    th  = ds18b20_r();
    dat = th;
    dat <<= 8;
    dat |= tl;

    return dat;
}
s16 ds18b20_convert_get_tem(u16 tem)
{
    s16 contem;
    float tms;
    if (tem >> 11 == 0x1f) {
        tem    = tem & 0x07ff;
        tem    = ~tem;
        tms    = (tem + 1) * 0.0625;
        contem = (u16)(tms * 10);
    } else {
        tms    = tem * 0.0625;
        contem = (u16)(tms * 10);
    }
    return contem;
}
void ds18b20_convert()
{
    ds18b20_w(SKIP_ROM);
    ds18b20_w(CONVERT_TEM);
}