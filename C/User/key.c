#include "key.h"

#define TEM_BTN    0x44
#define CFG_BTN    0x4c
#define PLUS_BTN   0x54
#define REDUCE_BTN 0x5c
#define LED_BTN    0x64

extern bool KEY_DOWN;
extern bool WARUP_STATUS;
extern bool LED_STATUS;
extern bool HF_STATUS_CFG;
extern bool HF_STATUS;
extern u8 LED_PWM_MIN_CFG;
extern u8 FAN_PWM_CFG;
extern u16 TEM_CFG;

void CFG_HANDLE();
void SET_CFG_TEM();
void SET_LED_PWM_MIN();
void SET_FAN_PWM();
void SET_HF_STATUS();

void KEY_HANDLE(u8 key)
{
    u8 dat;
    delay_ms(10);
    if (key == tm1650_readkey()) {
        switch (key) {
            case TEM_BTN:
                KEY_DOWN     = TRUE;
                WARUP_STATUS = !WARUP_STATUS;
                dat          = (WARUP_STATUS) ? 0xa1 : 0x00;
                eeprom_write(WARUP_STATUS_ADDRESS, dat);
                break;
            case LED_BTN:
                KEY_DOWN    = TRUE;
                LED_STATUS  = !LED_STATUS;
                dat         = (LED_STATUS) ? 0xa1 : 0x00;
                PA_ODR_ODR1 = (LED_STATUS) ? 0 : 1;
                eeprom_write(LED_STATUS_ADDRESS, dat);
                break;
            case CFG_BTN:
                KEY_DOWN = TRUE;
                tm1650_on();
                if (!WARUP_STATUS) {
                    CFG_HANDLE();
                }
                break;
        }
    }
}

void CFG_HANDLE()
{
    u8 display, i, key;
    i       = 0;
    display = 0;
    display = (display == 0x00) ? 0x11 : display;
    i       = (i == 0) ? 1 : i;
    bool w  = FALSE;
    while (!w) {
        switch (display) {
            case 0x11:
                tm1650_write(0, 19, FALSE);
                tm1650_write(1, 16, FALSE);
                tm1650_write(2, 1, FALSE);
                break;
            case 0x22:
                tm1650_write(0, 19, FALSE);
                tm1650_write(1, 16, FALSE);
                tm1650_write(2, 2, FALSE);
                break;
            case 0x33:
                tm1650_write(0, 19, FALSE);
                tm1650_write(1, 16, FALSE);
                tm1650_write(2, 3, FALSE);
                break;
            case 0x44:
                tm1650_write(0, 19, FALSE);
                tm1650_write(1, 16, FALSE);
                tm1650_write(2, 4, FALSE);
                break;
        }
        if (!KEY_DOWN) {
            key = tm1650_readkey();
            delay_ms(10);
            if (key == tm1650_readkey()) {
                switch (key) {
                    case CFG_BTN:
                        KEY_DOWN = TRUE;
                        i++;
#ifdef _HFON
                        i = (i > 4) ? 1 : i;
#else
                        i = (i > 3) ? 1 : i;
#endif
                        switch (i) {
                            case 1:
                                display = 0x11;
                                break;
                            case 2:
                                display = 0x22;
                                break;
                            case 3:
                                display = 0x33;
                                break;
                            case 4:
                                display = 0x44;
                                break;
                        }
                        break;
                    case LED_BTN:
                        KEY_DOWN = TRUE;
                        switch (display) {
                            case 0x11:
                                SET_CFG_TEM();
                                break;
                            case 0x22:
                                SET_LED_PWM_MIN();
                                if (LED_STATUS) {
                                    TIM2_CCR1H = 0;
                                    TIM2_CCR1L = 100;
                                } else {
                                    TIM2_CCR1H = 0;
                                    TIM2_CCR1L = 0;
                                }
                                break;
                            case 0x33:
                                SET_FAN_PWM();
                                break;
                            case 0x44:
                                SET_HF_STATUS();
                                break;
                        }
                        w = TRUE;
                        break;
                }
            }
        }
    }
}

void SET_CFG_TEM()
{
    u16 THE_TEM_CFG = TEM_CFG;
    bool w          = FALSE;
    u8 key;
    while (!w) {
        tm1650_write(0, THE_TEM_CFG / 100 % 100, FALSE);
        tm1650_write(1, THE_TEM_CFG / 10 % 10, TRUE);
        tm1650_write(2, THE_TEM_CFG % 10, FALSE);
        if (!KEY_DOWN) {
            key = tm1650_readkey();
            delay_ms(10);
            if (key == tm1650_readkey()) {
                switch (key) {
                    case CFG_BTN:
                        KEY_DOWN = TRUE;
                        w        = TRUE;
                        break;
                    case PLUS_BTN:
                        KEY_DOWN = TRUE;
                        THE_TEM_CFG++;
                        THE_TEM_CFG = (THE_TEM_CFG > 500) ? 500 : THE_TEM_CFG;
                        break;
                    case REDUCE_BTN:
                        KEY_DOWN = TRUE;
                        THE_TEM_CFG--;
                        THE_TEM_CFG = (THE_TEM_CFG < 100) ? 100 : THE_TEM_CFG;
                        break;
                    case LED_BTN:
                        KEY_DOWN = TRUE;
                        if (TEM_CFG != THE_TEM_CFG) {
                            TEM_CFG = THE_TEM_CFG;
                            eeprom_write16(TEM_ADDRESS, TEM_CFG);
                        }
                        w = TRUE;
                        break;
                }
            }
        }
    }
}
void SET_LED_PWM_MIN()
{
    u8 key;
    bool w = FALSE;
    u8 THE_LED_PWM_MIN;
    THE_LED_PWM_MIN = LED_PWM_MIN_CFG;
    while (!w) {
        u8 g, s;
        g = LED_PWM_MIN_CFG / 100 % 100;
        s = LED_PWM_MIN_CFG / 10 % 10;
        if (g > 0) {
            tm1650_write(0, g, FALSE);
        } else {
            tm1650_write(0, 19, FALSE);
        }
        if (s > 0) {
            tm1650_write(1, s, FALSE);
        } else {
            if (g > 0) {
                tm1650_write(1, s, FALSE);
            } else {
                tm1650_write(1, 19, FALSE);
            }
        }
        tm1650_write(2, LED_PWM_MIN_CFG % 10, FALSE);
        if (!KEY_DOWN) {
            key = tm1650_readkey();
            delay_ms(10);
            if (key == tm1650_readkey()) {
                switch (key) {
                    case CFG_BTN:
                        LED_PWM_MIN_CFG = THE_LED_PWM_MIN;
                        KEY_DOWN        = TRUE;
                        w               = TRUE;
                        break;
                    case PLUS_BTN:
                        KEY_DOWN = TRUE;
                        LED_PWM_MIN_CFG++;
                        LED_PWM_MIN_CFG = (LED_PWM_MIN_CFG > 100) ? 100 : LED_PWM_MIN_CFG;
                        break;
                    case REDUCE_BTN:
                        KEY_DOWN = TRUE;
                        if (LED_PWM_MIN_CFG > 0) {
                            LED_PWM_MIN_CFG--;
                        }
                        break;
                    case LED_BTN:
                        KEY_DOWN = TRUE;
                        if (LED_PWM_MIN_CFG != THE_LED_PWM_MIN) {
                            eeprom_write(LED_PWM_MIN_ADDRESS, LED_PWM_MIN_CFG);
                        }
                        w = TRUE;
                        break;
                }
            }
        }
    }
}
void SET_FAN_PWM()
{
    u8 key;
    bool w         = FALSE;
    u8 THE_FAN_PWM = FAN_PWM_CFG;
    TIM2_CCR3H     = 0;
    TIM2_CCR3L     = THE_FAN_PWM;
    while (!w) {
        if (THE_FAN_PWM < 100) {
            tm1650_write(0, 19, FALSE);
        } else {
            tm1650_write(0, THE_FAN_PWM / 100 % 100, FALSE);
        }
        tm1650_write(1, THE_FAN_PWM / 10 % 10, FALSE);
        tm1650_write(2, THE_FAN_PWM % 10, FALSE);
        if (!KEY_DOWN) {
            key = tm1650_readkey();
            delay_ms(10);
            if (key == tm1650_readkey()) {
                switch (key) {
                    case CFG_BTN:
                        KEY_DOWN = TRUE;
                        w        = TRUE;
                        break;
                    case PLUS_BTN:
                        KEY_DOWN = TRUE;
                        THE_FAN_PWM++;
                        THE_FAN_PWM = (THE_FAN_PWM > 100) ? 100 : THE_FAN_PWM;
                        TIM2_CCR3H  = 0;
                        TIM2_CCR3L  = THE_FAN_PWM;
                        break;
                    case REDUCE_BTN:
                        KEY_DOWN = TRUE;
                        if (THE_FAN_PWM > 60) {
                            THE_FAN_PWM--;
                        }
                        TIM2_CCR3H = 0;
                        TIM2_CCR3L = THE_FAN_PWM;
                        break;
                    case LED_BTN:
                        KEY_DOWN = TRUE;
                        if (THE_FAN_PWM != FAN_PWM_CFG) {
                            FAN_PWM_CFG = THE_FAN_PWM;
                            eeprom_write(FAN_PWM_ADDRESS, FAN_PWM_CFG);
                        }
                        w = TRUE;
                        break;
                }
            }
        }
    }
}
void SET_HF_STATUS()
{
    bool THE_HF_STATUS = HF_STATUS_CFG;
    u8 key;
    bool w = FALSE;
    while (!w) {
        if (THE_HF_STATUS) {
            tm1650_write(0, 19, FALSE);
            tm1650_write(1, 0, FALSE);
            tm1650_write(2, 18, FALSE);
        } else {
            tm1650_write(0, 0, FALSE);
            tm1650_write(1, 15, FALSE);
            tm1650_write(2, 15, FALSE);
        }
        if (!KEY_DOWN) {
            key = tm1650_readkey();
            delay_ms(10);
            if (key == tm1650_readkey()) {
                switch (key) {
                    case CFG_BTN:
                        KEY_DOWN = TRUE;
                        w        = TRUE;
                        break;
                    case PLUS_BTN:
                        KEY_DOWN      = TRUE;
                        THE_HF_STATUS = !THE_HF_STATUS;
                        break;
                    case REDUCE_BTN:
                        KEY_DOWN      = TRUE;
                        THE_HF_STATUS = !THE_HF_STATUS;
                        break;
                    case LED_BTN:
                        KEY_DOWN = TRUE;
                        u8 dat   = (THE_HF_STATUS) ? 0xa1 : 0x00;
                        if (THE_HF_STATUS != HF_STATUS_CFG) {
                            HF_STATUS_CFG = THE_HF_STATUS;
                            eeprom_write(HF_STATUS_ADDRESS, dat);
                        }
                        w = TRUE;
                        break;
                }
            }
        }
    }
}