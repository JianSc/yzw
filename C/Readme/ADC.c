// ADC 通道寄存器 (3=AIN3)
ADC_CSR_CH = 3;
do {
    // 开启转换
    ADC_CR1_ADON = 1;
    delay_us(5);
} while (RESET == ADC_CSR_EOC); // 等待转换结束
vcc25 = ADC_DRH << 2;           // 数据左对齐，高位在前
vcc25 |= ADC_DRL;
ADC_CSR_EOC = 0; // 清除标志位