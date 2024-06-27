#ifndef _USER_CONFIG_H
#define _USER_CONFIG_H

#include "iostm8s003f3.h"
#include "stm8s.h"

// 开启换风功能
// #define FAN_HF_START

#define TEMP_PLAY_LED_PORT GPIOC
#define TEMP_STOP_LED_PORT GPIOC
#define TEMP_PLAY_LED_PIN GPIO_PIN_5
#define TEMP_STOP_LED_PIN GPIO_PIN_6
#define FAN_PORT GPIOC
#define FAN_PIN GPIO_PIN_4
#define SW_PORT GPIOC
#define SW_PIN GPIO_PIN_3
#define FAN_LED_PORT GPIOD
#define FAN_LED_PIN GPIO_PIN_6
#define LED_LED_PORT GPIOA
#define LED_LED_PIN GPIO_PIN_1
#define LED_PORT GPIOD
#define LED_PIN GPIO_PIN_2
#define HF_PORT GPIOA
#define HF_PIN GPIO_PIN_2
#define MSD_PORT GPIOC
#define MSD_PIN GPIO_PIN_4
#define TEM_LED_EN   \
    PC_ODR_ODR5 = 0; \
    PC_ODR_ODR6 = 1
#define TEM_LED_CL   \
    PC_ODR_ODR5 = 1; \
    PC_ODR_ODR6 = 0
#define LED_R_EN PC_ODR_ODR5 = 0
#define LED_R_CL PC_ODR_ODR5 = 1
#define LED_G_EN PC_ODR_ODR6 = 0
#define LED_G_CL PC_ODR_ODR6 = 1
#define SW_EN PC_ODR_ODR3 = 1
#define SW_CL PC_ODR_ODR3 = 0
#define FAN_LED_EN PD_ODR_ODR6 = 0
#define FAN_LED_CL PD_ODR_ODR6 = 1
#define LED_LED_EN PA_ODR_ODR1 = 0
#define LED_LED_CL PA_ODR_ODR1 = 1
#define HF_EN PA_ODR_ODR2 = 1
#define HF_CL PA_ODR_ODR2 = 0
#define MSD_EN PC_ODR_ODR4 = 1
#define MSD_CL PC_ODR_ODR4 = 0

#include "stm8s_delay.h"
#include "tm1650.h"
#include "ds18b20.h"
#include "flash.h"
#include "ntc.h"
#include "key.h"

void init();

#endif
