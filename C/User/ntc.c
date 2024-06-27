#include "ntc.h"

//参考电压
float RefVol = 2.5;
// NTC阻值
float rt = 10000;
//分压电阻阻值, 分压电阻接VCC, NTC接GND
float rp = 1000;
// NTC B值
float bx = 3950;
float t2 = 273.15 + 25;
float ka = 273.15;

void ntcDataFilter(float *data, u8 data_len)
{
    float t;
    u8 i, j;
    for (i = 0; i < data_len; i++)
    {
        for (j = i + 1; j < data_len; j++)
        {
            if (data[i] < data[j])
            {
                t = data[j];
                data[j] = data[i];
                data[i] = t;
            }
        }
    }
}

float ntcReadNtcRes(u16 VccAdcNum, u16 ntcAdcNum)
{
    float t, vol;
    float vccvol;
    vol = RefVol / VccAdcNum;
    vccvol = vol * 1024;
    t = vol * ntcAdcNum;
    t = t / ((vccvol - t) / rp);
    return t;
}

float ntcReadNtcTemp(float ntcRes)
{

    float t;
    t = (1/(log(ntcRes/rt)/bx+(1/t2)))-273.15+0.5;
    return t;
}

float ntcReadNtcTemp_ave(float *data)
{
    float d;
    d = data[3] + data[4] + data[5] + data[6];
    d = d / 4;
    return d;
}