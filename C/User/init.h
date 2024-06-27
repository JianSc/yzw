#ifndef _INIT_H
#define _INIT_H

#include "stm8s003f3.h"
#include "stm8s.h"

// User header file
#include "tm1650.h"
#include "ds18b20.h"
#include "ntc.h"
#include "eeprom.h"
#include "key.h"

// 换气开关 (_HFOFF) 关闭换气功能, (_HFON) 开启换气功能
#define _HFON

#define TEM_ADDRESS          0x4000
#define LED_PWM_MIN_ADDRESS  0x4002
#define WARUP_STATUS_ADDRESS 0x4003
#define LED_STATUS_ADDRESS   0x4004
#define FAN_PWM_ADDRESS      0x4005
#define HF_STATUS_ADDRESS    0x4006

#define LED_D_ON             PA_ODR_ODR1 = 0
#define LED_D_OFF            PA_ODR_ODR1 = 1
// #define LED_ON    PD_ODR_ODR4 = 1
// #define LED_OFF   PD_ODR_ODR4 = 0
#define FAN_D_ON  PD_ODR_ODR6 = 0
#define FAN_D_OFF PD_ODR_ODR6 = 1
#define HF_ON     PA_ODR_ODR2 = 1
#define HF_OFF    PA_ODR_ODR2 = 0
// #define FAN_ON    PA_ODR_ODR3 = 1
#define FAN_OFF     \
    TIM2_CCR3H = 0; \
    TIM2_CCR3L = 0
// #define FAN_OFF   PA_ODR_ODR3 = 0
#define WARMUP_LED_ON \
    PC_ODR_ODR5 = 0;  \
    PC_ODR_ODR6 = 1
#define WARMUP_LED_OFF \
    PC_ODR_ODR5 = 1;   \
    PC_ODR_ODR6 = 0
#define MSD_ON  PC_ODR_ODR4 = 1
#define MSD_OFF PC_ODR_ODR4 = 0
#define SW_ON   PC_ODR_ODR3 = 1
#define SW_OFF  PC_ODR_ODR3 = 0

void init();
void delay_us(u16 us);
void delay_ms(u16 ms);

#endif