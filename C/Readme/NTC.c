// 开启ADC前关闭总中断

#define READ_ADC_LEN 10

// 获取10次 ADC的值
for (i = 0; i < READ_ADC_LEN; i++) {
    tems[i] = ntcReadInputRes();
}
// 冒泡算法, 数字滤波。提高ADC的精度
ntcDataFilter(tems, READ_ADC_LEN);
// 计算温度
ntctem = (s16)(ntcReadNtcTemp(ntcReadNtcTemp_ave(tems)) * 10);