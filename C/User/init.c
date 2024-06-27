#include "init.h"

void init()
{
    // 开启 HSI 与 LSI (内部高速与低速时钟)
    CLK->ICKR |= (u8)(1);
    CLK->ICKR |= (u8)(1 << 3);
    // 主时钟与CPU时钟不分频
    CLK->CKDIVR = 0x00;

    // 初始化IO引脚说明
    // PA_ODR_ODR1  GPIOA PIN1 复位(0) (0)置低电位，(1)置高电位
    // PA_DDR_DDR1  GPIOA PIN1 复位(0) (0)置输入，(1)置输出，在改变IO输入输出状态时，最好先ODR置电位，否则会产生抖动
    // PA_CR1_C11  GPIOA PIN1 复位(0) 输入时：(0)浮空 (1)带上拉，输出时：(0)开漏 (1)推挽
    // PA_CR2_C21  GPIOA PIN1 复位(0) 输入时:(0)禁止外部中断 (1)开启外部中断，输出时：(0)低速2Mhz (1)高速10Mhz
    // PA_IDR_IDR1 GPIOA PIN1 复位(0) 不论输入输出，都可以读取引脚状态

    PA_ODR_ODR1 = 1;
    PA_DDR_DDR1 = 1;
    PA_DDR_DDR2 = 1;
    PA_DDR_DDR3 = 1;
    PC_DDR_DDR3 = 1;
    // PC_ODR_ODR4 = 1;
    // PC_DDR_DDR4 = 1;
    PC_ODR_ODR5 = 1;
    PC_DDR_DDR5 = 1;
    PC_ODR_ODR6 = 1;
    PC_DDR_DDR6 = 1;
    PD_DDR_DDR3 = 0;
    PD_DDR_DDR2 = 0;
    PD_ODR_ODR6 = 1;
    PD_DDR_DDR6 = 1;

    // DS18b20
    PC_ODR_ODR7 = 1;
    PC_DDR_DDR7 = 1;

    // TM1650
    PB_ODR_ODR4 = 1;
    PB_DDR_DDR4 = 1;
    PB_ODR_ODR5 = 1;
    PB_DDR_DDR5 = 1;

    // ADC 初始化
    ADC_CR1_SPSEL = 4;
    ADC_TDRL |= (u8)(1 << 4);
    ADC_TDRL |= (u8)(1 << 3);

    // TIM2 初始化
    // 16Mhz/2^ , (0)不为频，(1)2分频, (2)4分频，(3)8分频, (4)16分频, (5)32分频, (6)64分频, (7)128分频, (8)256分频
    TIM2_PSCR_PSC = 7; // 分频128
    TIM2_CR1_CEN  = 1; // 使能记数器
    TIM2_IER_UIE  = 1;
    TIM2_ARRH     = 0;   // 自动装载值高位
    TIM2_ARRL     = 100; // 自动装载值低位

    // TIM2 PWM通道1
    TIM2_CCER1_CC1E  = 1; // 开启 PWM信号输出到对应引脚
    TIM2_CCMR1_OC1M  = 6; // (6)PWM模式1, 高位为占空比, (7)PWM模式2, 低位为占空比
    TIM2_CCR1H       = 0;
    TIM2_CCR1L       = 0; // 占空比, 100%为自动装载值
    TIM2_CCMR1_OC1PE = 1; // 开启预装载使能

    TIM2_CCER2_CC3E  = 1;
    TIM2_CCMR3_OC3M  = 6;
    TIM2_CCR3H       = 0;
    TIM2_CCR3L       = 0;
    TIM2_CCMR3_OC3PE = 1;
}

void delay_us(u16 us)
{
    while (us--) {
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
    }
}
void delay_ms(u16 ms)
{
    u16 i;
    while (ms--) {
        for (i = 900; i > 0; i--) {
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
        }
    }
}