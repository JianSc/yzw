#ifndef _DS18B20_H
#define _DS18B20_H

#include "init.h"

#define READ_ROM    0x33 // 读取序列号
#define SKIP_ROM    0xCC // 跳过ROM
#define MATCH_ROM   0x55 // 匹配ROM
#define CONVERT_TEM 0x44 // 转换温度
#define READ_RAM    0xBE // 读暂存器
#define WRITE_RAM   0x4E // 写暂存器

// 初始化 DS18B20 DQ 引脚 (init.c)
// GPIOC PIN7
// PC_ODR_ODR7 = 1;
// PC_DDR_DDR7 = 1;
// PC_CR2_C27  = 1;

#define DS18B20_DQ PC_ODR_ODR7
#define DS18B20_IO_IN \
    PC_CR2_C27  = 0;  \
    PC_DDR_DDR7 = 0
#define DS18B20_IO_OUT \
    PC_CR2_C27  = 1;   \
    PC_DDR_DDR7 = 1
#define DS18B20_DQ_READ PC_IDR_IDR7

bool ds18b20_init();
u16 ds18b20_read();
s16 ds18b20_convert_get_tem(u16 tem);
void ds18b20_convert();

#endif