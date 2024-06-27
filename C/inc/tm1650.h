#ifndef _TM1650_H
#define _TM1650_H

#include "init.h"

// 初始化 TM1650 IO引脚 (init.c)
// PB_ODR_ODR4 = 1;
// PB_DDR_DDR4 = 1;
// PB_CR2_C24  = 1;
// PB_ODR_ODR5 = 1;
// PB_DDR_DDR5 = 1;
// PB_CR2_C25  = 1;

// TODO:TM1650-IO设置

#define TM1650_SCL        PB_ODR_ODR4
#define TM1650_SDA        PB_ODR_ODR5
#define TM1650_SDA_READ   PB_IDR_IDR5
#define TEM1650_SDAIO_IN  PB_DDR_DDR5 = 0
#define TEM1650_SDAIO_OUT PB_DDR_DDR5 = 1

// addr为通道, dat为显示数据或命令参数
void tm1650_write(u8 addr, u8 dat, bool dp);
// 获取按键
u8 tm1650_readkey();
// TM1650初始化
void tm1650_init();
// 开启输出
void tm1650_on();
// 关闭输出
void tm1650_off();

#endif