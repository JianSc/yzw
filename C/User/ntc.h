#ifndef NTC_H
#define NTC_H
#include "init.h"

//冒泡排序算法, 数字滤波, 提高NTC精度(data_len 为 *data 数组高度,也就是ADC采集的次数)
void ntcDataFilter(float *data, u8 data_len);
//计算NTC阻值
// ADC获取两次，第一次为获取参考通道的ADC值(VccAdcNum), 第二次为获取NTC通道ADC值(ntcAdcNum)
//传入参数计算NTC阻值;
float ntcReadNtcRes(u16 VccAdcNum, u16 ntcAdcNum);
//计算温度平均值, 数字滤波 (*data 为ntcDataFiler() 冒泡排序的值)
float ntcReadNtcTemp_ave(float *data);
//返回NTC温度数据*(ntcRes 可以是 ntcReadNtcTemp_ave() 的平均值，也可以是阻值，)
float ntcReadNtcTemp(float ntcRes);
#endif