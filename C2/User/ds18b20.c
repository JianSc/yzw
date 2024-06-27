#include "ds18b20.h"

// 频率: 16Mhz 延时函数
// void delay_us(u16 nus)
// {
//     u16 i = 0, j = 0;
//     for (i = 0; i < (nus >> 2); i++)
//     {
//         for (j = 0; j < 7; j++)
//             ;
//     }
// }

bool DS18B20_Init()
{
    u8 retry = 0;
    DS18B20_IO_OUT;
    DS18B20_DQ_CL;
    delay_us(600);
    DS18B20_DQ_EN;
    delay_us(10);
    DS18B20_IO_IN;
    while (DS18B20_DQ_IN && retry < 200)
    {
        retry++;
        delay_us(1);
    }
    if (retry >= 200)
    {
        return FALSE;
    }
    else
    {
        retry = 0;
    }
    while (!DS18B20_DQ_IN && retry < 240)
    {
        retry++;
        delay_us(1);
    }
    if (retry >= 240)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

uint8_t DS18B20_R()
{
    uint8_t d, i;
    for (i = 0; i < 8; i++)
    {
        d >>= 1;
        DS18B20_IO_OUT;
        DS18B20_DQ_CL;
        delay_us(2);
        DS18B20_DQ_EN;
        DS18B20_IO_IN;
        delay_us(12);
        if (DS18B20_DQ_IN)
        {
            d |= 0x80;
        }
        delay_us(50);
    }
    return d;
}

void DS18B20_W(uint8_t d)
{
    uint8_t i;
    DS18B20_IO_OUT;
    for (i = 0; i < 8; i++)
    {
        bool d2 = (d & 0x01) ? TRUE : FALSE;
        if (d2)
        {
            DS18B20_DQ_CL;
            delay_us(2);
            DS18B20_DQ_EN;
            delay_us(60);
        }
        else
        {
            DS18B20_DQ_CL;
            delay_us(60);
            DS18B20_DQ_EN;
            delay_us(2);
        }
        d >>= 1;
    }
}

// 读取温度
u16 DS18B20_Read()
{
    u16 dat;
    u8 th, tl;

    DS18B20_W(0xcc);
    DS18B20_W(0xbe);

    tl = DS18B20_R();
    th = DS18B20_R();
    dat = th;
    dat <<= 8;
    dat |= tl;

    return dat;
}

// 转换温度，实际温度 * 10 取整。
int DS18B20_Convert_Get_Tem(u16 temp)
{
    int contem;
    float tms;
    if (temp >> 11 == 0x1f)
    {
        temp = temp & 0x07ff;
        temp = ~temp;
        tms = (temp + 1) * 0.0625;
        contem = (int)(tms * 10);
    }
    else
    {
        tms = temp * 0.0625;
        contem = (int)(tms * 10);
    }
    return contem;
}

// 启动温度转换
void ds18b20_Conver()
{
    DS18B20_W(0xcc);
    DS18B20_W(0x44);
}