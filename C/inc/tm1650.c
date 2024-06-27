#include "tm1650.h"

// TODO:TM1650-LED码	   0	 1		2     3     4     5    6     7     8     9
// A(10)     B(11)     C(12)     D(13)     E(14)     F(15)	  P(16)     -(17)      n(18)        (19为空)
// 共阳
// uint8_t LED7Code[] = {~0x3f, ~0x06, ~0x5b, ~0x4f, ~0x66, ~0x6d, ~0x7d, ~0x07, ~0x7f, ~0x6f, ~0x77, ~0x7c, ~0x39, ~0x5e, ~0x79, ~0x71, ~0x73, ~0x40, ~0x37, ~0x00};
// 共阴
u8 LED7Code[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x73, 0x40, 0x37, 0x00};

// TODO:TM1650-亮度
u8 tm1650_level = 6;
// TODO:TM1650-片内缓存地址
u8 tm1650_addrs[4] = {0x68, 0x6a, 0x6c, 0x6e};
// TODO:TM1650-显示方式 (0x08) 7段不带DP, （0x00) 8段带DP
u8 tm1650_para = 0x00;

void tm1650_start()
{
    TM1650_SCL = 1;
    TM1650_SDA = 1;
    nop;
    TM1650_SDA = 0;
    nop;
    TM1650_SCL = 0;
    nop;
}

void tm1650_stop()
{
    TM1650_SCL = 0;
    TM1650_SDA = 0;
    nop;
    TM1650_SCL = 1;
    nop;
    TM1650_SDA = 1;
    nop;
}

bool ack()
{
    bool b;
    u16 retry  = 0;
    TM1650_SDA = 1;
    TEM1650_SDAIO_IN;
    nop;
    while (TM1650_SDA_READ && retry < 2000) {
        retry++;
        delay_us(1);
    }
    b = (retry < 2000) ? FALSE : TRUE;
    TEM1650_SDAIO_OUT;
    nop;
    TM1650_SCL = 1;
    nop;
    TM1650_SCL = 0;
    nop;
    return b;
}
void Nack()
{
    TM1650_SCL = 1;
    nop;
    TM1650_SCL = 0;
    nop;
}

void tm1650W(uint8_t dat)
{
    u8 i;
    for (i = 0; i < 8; i++) {
        TM1650_SDA = (dat & 0x80) ? 1 : 0;
        dat <<= 1;
        nop;
        TM1650_SCL = 1;
        nop;
        TM1650_SCL = 0;
        nop;
    }
}
u8 tm1650R()
{
    char i;
    u8 dat = 0x00;
    TEM1650_SDAIO_IN;
    nop;
    for (i = 0; i < 8; i++) {
        TM1650_SCL = 1;
        nop;
        dat <<= 1;
        if (TM1650_SDA_READ) {
            dat = dat | 0x01;
        }
        TM1650_SCL = 0;
        nop;
    }
    TEM1650_SDAIO_OUT;
    return dat;
}

void tm1650_init()
{
    u8 i;
    uint8_t lenght = sizeof(tm1650_addrs) / sizeof(tm1650_addrs[0]);
    tm1650_write(0x48, ((tm1650_level << 4) | tm1650_para) | 0x01, FALSE);
    for (i = 0; i < lenght; i++) {
        tm1650_write(tm1650_addrs[i], 0x40, FALSE);
    }
}
void tm1650_write(u8 addr, u8 dat, bool dp)
{
    if (addr < 4) {
        do {
            tm1650_start();
            tm1650W(tm1650_addrs[addr]);
        } while (ack());
        if (dp && tm1650_para == 0x00) {
            tm1650W(LED7Code[dat] | 0x80);
        } else {
            tm1650W(LED7Code[dat]);
        }
        Nack();
        tm1650_stop();
    } else {
        do {
            tm1650_start();
            tm1650W(addr);
        } while (ack());
        tm1650W(dat);
        Nack();
        tm1650_stop();
    }
}

u8 tm1650_readkey()
{
    u8 dat;
    do {
        tm1650_start();
        tm1650W(0x49);
    } while (ack());
    dat = tm1650R();
    Nack();
    tm1650_stop();
    return dat;
}

void tm1650_on()
{
    tm1650_write(0x48, ((tm1650_level << 4) | tm1650_para) | 0x01, FALSE);
}

void tm1650_off()
{
    tm1650_write(0x48, 0x00, FALSE);
}
