#include "tm1650.h"

// TODO:TM1650-亮度
uint8_t tm1650_level = 6;
// TODO:TM1650-片内缓存地址
uint8_t tm1650_addrs[4] = {0x68, 0x6a, 0x6c, 0x6e};

void tm1650_start()
{
    TM1650_SDA_EN;
    TM1650_SCL_EN;
    TM1650_SDA_CL;
    nop();
    TM1650_SCL_CL;
    nop();
}

void tm1650_stop()
{
    TM1650_SDA_CL;
    TM1650_SCL_EN;
    nop();
    TM1650_SDA_EN;
    nop();
}

bool ack()
{
    bool b = FALSE;
    u8 i = 0;
    TM1650_SDA_EN;
    do
    {
        i++;
        if (i > 200)
        {
            b = TRUE;
            continue;
        }
        delay_us(1);
    } while (READ_TM1650_KEY);
    TM1650_SCL_EN;
    TM1650_SCL_CL;
    return b;
}

void tm1650W(uint8_t dat)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
        {
            TM1650_SDA_EN;
        }
        else
        {
            TM1650_SDA_CL;
        }
        TM1650_SCL_EN;
        nop();
        TM1650_SCL_CL;
        dat <<= 1;
    }
}
uint8_t tm1650R()
{
    char i;
    uint8_t dat = 0x00;
    TM1650_SDA_EN;
    TM1650_SCL_CL;
    nop();
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        TM1650_SCL_EN;
        nop();
        if (READ_TM1650_KEY)
        {
            dat = dat | 0x01;
        }
        TM1650_SCL_CL;
        nop();
    }
    return dat;
}

void tm1650_displayInit()
{
    u8 i;
    uint8_t lenght = sizeof(tm1650_addrs) / sizeof(tm1650_addrs[0]);
    tm1650_displayW(0x48, (tm1650_level << 4) | 0x01);
    for (i = 0; i < lenght; i++)
    {
        tm1650_displayW(tm1650_addrs[i], 0x40);
    }
}
void tm1650_displayW(uint8_t addr, uint8_t dat)
{
    switch (addr)
    {
    case 0:
        do
        {
            tm1650_start();
            tm1650W(tm1650_addrs[0]);
        } while (ack());
        tm1650W(dat);
        ack();
        tm1650_stop();
        break;
    case 1:
        do
        {
            tm1650_start();
            tm1650W(tm1650_addrs[1]);
        } while (ack());
        tm1650W(dat);
        ack();
        tm1650_stop();
        break;
    case 2:
        do
        {
            tm1650_start();
            tm1650W(tm1650_addrs[2]);
        } while (ack());
        tm1650W(dat);
        ack();
        tm1650_stop();
        break;
    case 3:
        do
        {
            tm1650_start();
            tm1650W(tm1650_addrs[3]);
        } while (ack());
        tm1650W(dat);
        ack();
        tm1650_stop();
        break;
    default:
        do
        {
            tm1650_start();
            tm1650W(addr);
        } while (ack());
        tm1650W(dat);
        ack();
        tm1650_stop();
        break;
    }
}

uint8_t tm1650_displaykeyR()
{
    uint8_t dat;
    do
    {
        tm1650_start();
        tm1650W(0x49);
    } while (ack());
    dat = tm1650R();
    tm1650_stop();
    return dat;
}

void tm1650_displayOn()
{
    tm1650_displayW(0x48, (tm1650_level << 4) | 0x01);
}

void tm1650_displayOff()
{
    tm1650_displayW(0x48, 0x00);
}