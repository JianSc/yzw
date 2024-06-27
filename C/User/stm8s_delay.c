#include "stm8s_delay.h"

void delay_ms(u16 nus)
{
    TIM4_DeInit();
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 124);
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4_SetCounter(0);
    TIM4_Cmd(ENABLE);

    while (nus--)
    {
        while (SET != TIM4_GetFlagStatus(TIM4_FLAG_UPDATE))
            ;
        TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    }
    TIM4_Cmd(DISABLE);
}

void delay_us(u16 nus)
{
    u16 i = 0, j = 0;
    for (i = 0; i < (nus >> 2); i++)
    {
        for (j = 0; j < 7; j++)
            ;
    }
}