#ifndef DS18B20_H
#define DS18B20_H

#include "init.h"

#define READ_ROM 0x33    // 读取序列号
#define SKIP_ROM 0xCC    // 跳过ROM
#define MATCH_ROM 0x55   // 匹配ROM
#define CONVERT_TEM 0x44 // 转换温度
#define READ_RAM 0xBE    // 读暂存器
#define WRITE_RAM 0x4E   // 写暂存器

// TODO: DS18B20 - IO脚位选择---------------------------------------------------
// #define DS18B20_SDA_PORT GPIOC
// #define DS18B20_SDA_PIN GPIO_PIN_7

// #define DS18B20_DQ_EN GPIO_WriteHigh(DS18B20_SDA_PORT, DS18B20_SDA_PIN)
// #define DS18B20_DQ_CL GPIO_WriteLow(DS18B20_SDA_PORT, DS18B20_SDA_PIN)
// #define DS18B20_DQ_IN GPIO_ReadInputPin(DS18B20_SDA_PORT, DS18B20_SDA_PIN)
// #define DS18B20_IO_IN GPIO_Init(DS18B20_SDA_PORT, DS18B20_SDA_PIN, GPIO_MODE_IN_PU_NO_IT)
// #define DS18B20_IO_OUT GPIO_Init(DS18B20_SDA_PORT, DS18B20_SDA_PIN, GPIO_MODE_OUT_OD_HIZ_FAST)

#define DS18B20_IO_IN PC_DDR_DDR7 = 0  // PC7   置输入
#define DS18B20_IO_OUT PC_DDR_DDR7 = 1 // PC7   置输出
#define DS18B20_DQ_IN PC_IDR_IDR7      // PC7   IO获取
#define DS18B20_DQ_EN PC_ODR_ODR7 = 1  // 拉高
#define DS18B20_DQ_CL PC_ODR_ODR7 = 0  // 拉低

bool DS18B20_Init(); // 初始化

u16 DS18B20_Read();                    // 读取温度
int DS18B20_Convert_Get_Tem(u16 temp); // 转换温度，实际温度 * 10 取整。
void ds18b20_Conver();                 // 启动温度转换
#endif