#include "init.h"

#define READ_ADC_LEN 10

// A(10)     B(11)     C(12)     D(13)     E(14)     F(15)	  P(16)     -(17)      n(18)        (19为空)
// (P1)温度设置 (P2)LED最小亮度设置 (P3)风扇转速 (P4)换气开关

bool TEM_GET_STATUS, WARUP_STATUS, LED_STATUS, HF_STATUS_CFG, HF_STATUS;
bool LCD_STATUS;
u16 TEM_CFG;
u8 LED_PWM_MIN_CFG, FAN_PWM_CFG;
bool KEY_DOWN;

void main()
{
    float tems[10];
    s16 tem, ntctem;

    init();
    tm1650_init();
    delay_ms(500);

    WARUP_STATUS    = FALSE;
    LED_STATUS      = FALSE;
    TEM_CFG         = eeprom_read16(TEM_ADDRESS, 310);
    LED_PWM_MIN_CFG = eeprom_read(LED_PWM_MIN_ADDRESS, 0);
    FAN_PWM_CFG     = eeprom_read(FAN_PWM_ADDRESS, 95);
    WARUP_STATUS    = (eeprom_read(WARUP_STATUS_ADDRESS, 0) == 0xa1) ? TRUE : FALSE;
    LED_STATUS      = (eeprom_read(LED_STATUS_ADDRESS, 0) == 0xa1) ? TRUE : FALSE;
    HF_STATUS_CFG   = (eeprom_read(HF_STATUS_ADDRESS, 0) == 0xa1) ? TRUE : FALSE;

    WARMUP_LED_OFF;

    while (1) {
        u8 i;
        // 检测温度
        if (!TEM_GET_STATUS) {
            disableInterrupts();
            // DS18B20
            while (!ds18b20_init()) {}
            ds18b20_convert();
            while (!ds18b20_init()) {}
            tem = ds18b20_convert_get_tem(ds18b20_read());

            // NTC
            for (i = 0; i < READ_ADC_LEN; i++) {
                tems[i] = ntcReadInputRes();
            }
            ntcDataFilter(tems, READ_ADC_LEN);
            ntctem = (s16)(ntcReadNtcTemp(ntcReadNtcTemp_ave(tems)) * 10);

            // 显示温度
            s16 xtem = tem;
            xtem     = (xtem < -99) ? -99 : xtem;
            xtem     = (xtem > 999) ? 999 : xtem;
            if (xtem < 0) {
                tm1650_write(0, 17, FALSE);
                xtem = ~xtem + 1;
                tm1650_write(1, xtem / 10 % 10, TRUE);
                tm1650_write(2, xtem % 10, FALSE);
            } else {
                if (xtem < 100) {
                    tm1650_write(0, 19, FALSE);
                } else {
                    tm1650_write(0, xtem / 100 % 100, FALSE);
                }
                tm1650_write(1, xtem / 10 % 10, TRUE);
                tm1650_write(2, xtem % 10, FALSE);
            }

            TEM_GET_STATUS = TRUE;
            enableInterrupts();
        }

        // 匹配按键
        if (!KEY_DOWN) {
            u8 key = tm1650_readkey();
            KEY_HANDLE(key);
        }

        if (HF_STATUS_CFG) {
            // 过温
            if (tem > TEM_CFG + 5) {
                SW_OFF;
                WARMUP_LED_OFF;
                FAN_OFF;
                HF_ON;
                continue;
            } else {
                if (!HF_STATUS) {
                    HF_OFF;
                }
            }

            // 换气
            if (HF_STATUS) {
                if (LCD_STATUS) {
                    tm1650_on();
                } else {
                    tm1650_off();
                }
                SW_OFF;
                WARMUP_LED_OFF;
                FAN_OFF;
                HF_ON;
                continue;
            } else {
                HF_OFF;
                tm1650_on();
                LCD_STATUS = FALSE;
            }
        } else {
            HF_OFF;
        }
        // 加温
        if (WARUP_STATUS) {

            TIM2_CCR3H = 0;
            TIM2_CCR3L = FAN_PWM_CFG;
            FAN_D_ON;

            u16 warup_tem = 1000;
            for (u8 x = 20; x > 0; x--) {
                warup_tem = ((TEM_CFG - x) < tem) ? (TEM_CFG + (u16)((float)((float)x / 20) * 600)) : warup_tem;
            }
            warup_tem = (TEM_CFG <= tem) ? TEM_CFG + 5 : warup_tem;

            if (ntctem < warup_tem) {
                WARMUP_LED_ON;
                SW_ON;
            } else {
                WARMUP_LED_OFF;
                SW_OFF;
            }
        } else {
            WARMUP_LED_OFF;
            SW_OFF;
            FAN_D_OFF;
            TIM2_CCR3H = 0;
            TIM2_CCR3L = 0;
        }
    }
}

#pragma vector = TIM2_OVR_UIF_vector
__interrupt void TIM2_UPD_OVF_BRK_IRQHandler(void)
{
    static u16 TEM_STATUS_REF_TIME, HF_TIME_S;
    static u8 HF_TIME_M, HF_TIME_H, LED_PWM_TIME, LED_PWM;
    static u16 KEY_DOWN_TIME, LCD_STATUS_TIME;
    // 温度采集缓冲时间
    if (TEM_GET_STATUS) {
        if (TEM_STATUS_REF_TIME < 100) {
            TEM_STATUS_REF_TIME++;
        } else {
            TEM_STATUS_REF_TIME = 0;
            TEM_GET_STATUS      = FALSE;
        }
    }
    // 按鈕按下復位
    if (KEY_DOWN) {
        if (KEY_DOWN_TIME < 400) {
            KEY_DOWN_TIME++;
        } else {
            KEY_DOWN_TIME = 0;
            KEY_DOWN      = FALSE;
        }
    }
    // LED
    if (LED_STATUS) {
        if (LED_PWM_TIME < 30) {
            LED_PWM_TIME++;
        } else {
            LED_PWM_TIME = 0;
            LED_PWM++;
            LED_PWM    = (LED_PWM > 100) ? 100 : LED_PWM;
            TIM2_CCR1H = 0;
            TIM2_CCR1L = LED_PWM;
        }
    } else {
        if (LED_PWM_TIME < 30) {
            LED_PWM_TIME++;
        } else {
            LED_PWM_TIME = 0;
            if (LED_PWM > LED_PWM_MIN_CFG) {
                LED_PWM--;
            }
            if (LED_PWM < LED_PWM_MIN_CFG) {
                LED_PWM++;
            }
            TIM2_CCR1H = 0;
            TIM2_CCR1L = LED_PWM;
        }
    }
    // 换气时间
    if (HF_STATUS_CFG) {
        if (HF_STATUS) {
            if (LCD_STATUS_TIME > 1000) {
                LCD_STATUS_TIME = 0;
                LCD_STATUS      = !LCD_STATUS;
            } else {
                LCD_STATUS_TIME++;
            }
            if (HF_TIME_S < 30000) {
                HF_TIME_S++;
            } else {
                HF_TIME_S = 0;
                HF_STATUS = FALSE;
            }
        } else {
            if (HF_TIME_S < 60000) {
                HF_TIME_S++;
            } else {
                HF_TIME_S = 0;
                HF_TIME_M++;
            }
            if (HF_TIME_M > 59) {
                HF_TIME_M = 0;
                HF_TIME_H++;
            }
            if (HF_TIME_H > 2) {
                HF_TIME_H = 0;
                HF_STATUS = TRUE;
            }
        }
    }

    TIM2_SR1_UIF = 0;
}
