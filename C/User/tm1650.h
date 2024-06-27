#ifndef TM1650_H
#define TM1650_H

#include "init.h"

// LED7codeH 共阳 , LED7codeL 共阴
// uint8_t LED7CodeH[] = {~0x3f, ~0x06, ~0x5b, ~0x4f, ~0x66, ~0x6d, ~0x7d, ~0x07, ~0x7f, ~0x6f, ~0x77, ~0x7c, ~0x39, ~0x5e, ~0x79, ~0x71, ~0x40};
// uint8_t LED7CodeL[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x40};

//TODO:TM1650-IO设置
#define TM1650_SCL_PORT GPIOB
#define TM1650_SDA_PORT GPIOB
#define TM1650_SCL_PIN GPIO_PIN_4
#define TM1650_SDA_PIN GPIO_PIN_5

#define TM1650_SCL_EN GPIO_WriteHigh(TM1650_SCL_PORT,TM1650_SCL_PIN)
#define TM1650_SCL_CL GPIO_WriteLow(TM1650_SCL_PORT,TM1650_SCL_PIN)
#define TM1650_SDA_EN GPIO_WriteHigh(TM1650_SDA_PORT,TM1650_SDA_PIN)
#define TM1650_SDA_CL GPIO_WriteLow(TM1650_SDA_PORT,TM1650_SDA_PIN)
#define READ_TM1650_KEY GPIO_ReadInputPin(TM1650_SDA_PORT,TM1650_SDA_PIN)

//addr为通道, dat为显示数据或命令参数
void tm1650_displayW(uint8_t addr, uint8_t dat);
//获取按键
uint8_t tm1650_displaykeyR();
//TM1650初始化
void tm1650_displayInit();
//开启输出
void tm1650_displayOn();
//关闭输出
void tm1650_displayOff();

#endif