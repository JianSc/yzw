#include "key.h"

// btn[0] 为启动/停止按钮, btn[1] 设置按钮, btn[2] +按钮， btn[3] -按钮, btn[4] 确定/灯光按钮.
// u8 btn[] = {0x44, 0x4c, 0x54, 0x5c, 0x64};
extern bool LED_STATUS;
extern bool CONFIG_STATUS;
extern uint8_t LED7CodeL[];
extern bool TEM_STATUS;
extern bool HF_STATUS;

extern int TEM_SUM;
extern u8 FAN_PWM_NUM;

bool KEY_DOWN;
u8 KEY_TYPE = 0x11;
bool CONFIG_LED_STATUS = FALSE;
bool syswhile = FALSE;

// TODO:TM1650-LED码	   0	 1		2     3     4     5    6     7     8     9     A     B     C     D     E     F	  P 	-
// LED7codeH 共阳 , LED7codeL 共阴
// uint8_t LED7CodeH[] = {~0x3f, ~0x06, ~0x5b, ~0x4f, ~0x66, ~0x6d, ~0x7d, ~0x07, ~0x7f, ~0x6f, ~0x77, ~0x7c, ~0x39, ~0x5e, ~0x79, ~0x71, ~0x73, ~0x40};
// uint8_t LED7CodeL[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x73, 0x40};

void config_tem();
void config_fan();
void config_hf();

void btn_down(u8 key)
{
    if (!KEY_DOWN)
    {
        switch (key)
        {
        case 0x64:
            delay_ms(10);
            if (tm1650_displaykeyR() == 0x64)
            {
                LED_STATUS = !LED_STATUS;
                FLASH_DATA_LED_STATUS_WRITE(LED_STATUS);
                KEY_DOWN = TRUE;
            }
            break;
        case 0x44:
            delay_ms(10);
            if (tm1650_displaykeyR() == 0x44)
            {
                TEM_STATUS = !TEM_STATUS;
                FLASH_DATA_TEM_STATUS_WRTIE(TEM_STATUS);
                KEY_DOWN = TRUE;
            }
            break;
        case 0x4c:
            if (!TEM_STATUS)
            {
                delay_ms(10);
                if (tm1650_displaykeyR() == 0x4c)
                {
                    KEY_DOWN = TRUE;
                    syswhile = TRUE;
                    tm1650_displayW(0, 0x00);
                    tm1650_displayW(1, LED7CodeL[16]);
                    while (syswhile)
                    {
                        if (!KEY_DOWN)
                        {
                            key = tm1650_displaykeyR();
                            switch (key)
                            {
                            case 0x4c:
                                delay_ms(10);
                                if (tm1650_displaykeyR() == 0x4c)
                                {
                                    static u8 i;
#ifdef FAN_HF_START
                                    i++;
                                    i = (i > 3) ? 1 : i;
                                    switch (i)
                                    {
                                    case 1:
                                        KEY_TYPE = 0x11;
                                        break;
                                    case 2:
                                        KEY_TYPE = 0x22;
                                        break;
                                    case 3:
                                        KEY_TYPE = 0x33;
                                        break;
                                    }
#else
                                    i++;
                                    i = (i > 2) ? 1 : i;
                                    switch (i)
                                    {
                                    case 1:
                                        KEY_TYPE = 0x11;
                                        break;
                                    case 2:
                                        KEY_TYPE = 0x22;
                                        break;
                                    }
#endif
                                    KEY_DOWN = TRUE;
                                }
                                break;
                            case 0x64:
                                delay_ms(10);
                                if (tm1650_displaykeyR() == 0x64)
                                {
                                    KEY_DOWN = TRUE;
                                    switch (KEY_TYPE)
                                    {
                                        // 设置温度
                                    case 0x11:
                                        config_tem();
                                        break;
                                        // 设置风扇转速
                                    case 0x22:
                                        config_fan();
                                        break;
                                        // 换气功能
                                    case 0x33:
                                        config_hf();
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                        switch (KEY_TYPE)
                        {
                        case 0x11:
                            tm1650_displayW(2, LED7CodeL[1]);
                            break;
                        case 0x22:
                            tm1650_displayW(2, LED7CodeL[2]);
                            break;
                        }
                    }
                }
            }
            break;
        }
    }
}

// 设置温度
// btn[0] 为启动/停止按钮, btn[1] 设置按钮, btn[2] +按钮， btn[3] -按钮, btn[4] 确定/灯光按钮.
// u8 btn[] = {0x44, 0x4c, 0x54, 0x5c, 0x64};
void config_tem()
{
    CONFIG_STATUS = TRUE;
    bool whileback = FALSE;
    int sum = TEM_SUM;
    while (1)
    {
        if (CONFIG_LED_STATUS)
        {
            tm1650_displayOn();
        }
        else
        {
            tm1650_displayOff();
        }
        tm1650_displayW(0, LED7CodeL[sum / 100 % 100]);
        tm1650_displayW(1, (LED7CodeL[sum / 10 % 10]) | 0x80);
        tm1650_displayW(2, LED7CodeL[sum % 10]);
        if (!KEY_DOWN)
        {
            u8 key = tm1650_displaykeyR();
            switch (key)
            {
            case 0x4c:
                delay_ms(10);
                if (tm1650_displaykeyR() == 0x4c)
                {
                    KEY_DOWN = TRUE;
                    whileback = TRUE;
                }
                break;
            case 0x64:
                delay_ms(10);
                if (tm1650_displaykeyR() == 0x64)
                {
                    KEY_DOWN = TRUE;
                    TEM_SUM = sum;
                    FLASH_DATA_TEM_SUM_WRITE(TEM_SUM);
                    whileback = TRUE;
                }
                break;
            case 0x54:
                delay_ms(10);
                if (tm1650_displaykeyR() == 0x54)
                {
                    KEY_DOWN = TRUE;
                    sum++;
                    sum = (sum > 500) ? 500 : sum;
                }
                break;
            case 0x5c:
                delay_ms(10);
                if (tm1650_displaykeyR() == 0x5c)
                {
                    KEY_DOWN = TRUE;
                    sum--;
                    sum = (sum < 100) ? 100 : sum;
                }
                break;
            }
        }
        if (whileback)
        {
            CONFIG_STATUS = FALSE;
            syswhile = FALSE;
            tm1650_displayOn();
            break;
        }
    }
}

// 设置风扇转速
void config_fan()
{
    CONFIG_STATUS = TRUE;
    bool whileback = FALSE;
    int sum = FAN_PWM_NUM;
    while (1)
    {
        if (CONFIG_LED_STATUS)
        {
            tm1650_displayOn();
        }
        else
        {
            tm1650_displayOff();
        }
        if ((sum / 100 % 100) == 0)
        {
            tm1650_displayW(0, 0x00);
        }
        else
        {
            tm1650_displayW(0, LED7CodeL[sum / 100 % 100]);
        }
        tm1650_displayW(1, LED7CodeL[sum / 10 % 10]);
        tm1650_displayW(2, LED7CodeL[sum % 10]);
        if (!KEY_DOWN)
        {
            u8 key = tm1650_displaykeyR();
            switch (key)
            {
            case 0x4c:
                delay_ms(10);
                if (tm1650_displaykeyR() == 0x4c)
                {
                    KEY_DOWN = TRUE;
                    whileback = TRUE;
                    TIM2_SetCompare3(FAN_PWM_NUM);
                }
                break;
            case 0x64:
                delay_ms(10);
                if (tm1650_displaykeyR() == 0x64)
                {
                    KEY_DOWN = TRUE;
                    FAN_PWM_NUM = sum;
                    FLASH_DATA_FAN_PWM_WRITE(FAN_PWM_NUM);
                    TIM2_SetCompare3(FAN_PWM_NUM);
                    whileback = TRUE;
                }
                break;
            case 0x54:
                delay_ms(10);
                if (tm1650_displaykeyR() == 0x54)
                {
                    KEY_DOWN = TRUE;
                    sum++;
                    sum = (sum > 100) ? 100 : sum;
                    TIM2_SetCompare3(sum);
                }
                break;
            case 0x5c:
                delay_ms(10);
                if (tm1650_displaykeyR() == 0x5c)
                {
                    KEY_DOWN = TRUE;
                    sum--;
                    sum = (sum < 50) ? 50 : sum;
                    TIM2_SetCompare3(sum);
                }
                break;
            }
        }
        if (whileback)
        {
            CONFIG_STATUS = FALSE;
            syswhile = FALSE;
            tm1650_displayOn();
            break;
        }
    }
}

// 开关换气功能
void config_hf()
{
    bool whileback = FALSE;
    bool bol_hf = HF_STATUS;
    tm1650_displayOn();
    while (1)
    {
        tm1650_displayW(0, 0x00);
        tm1650_displayW(1, LED7CodeL[14]);
        if (HF_STATUS)
        {
            tm1650_displayW(2, LED7CodeL[1]);
        }
        else
        {
            tm1650_displayW(2, LED7CodeL[0]);
        }

        if (!KEY_DOWN)
        {
            u8 key = tm1650_displaykeyR();
            switch (key)
            {
            case 0x4c:
                delay_ms(10);
                if (tm1650_displaykeyR() == 0x4c)
                {
                    KEY_DOWN = TRUE;
                    if (bol_hf)
                    {
                        bol_hf = FALSE;
                        tm1650_displayW(2, LED7CodeL[0]);
                    }
                    else
                    {
                        bol_hf = TRUE;
                        tm1650_displayW(2, LED7CodeL[1]);
                    }
                }
                break;
            case 0x64:
                delay_ms(10);
                if (tm1650_displaykeyR() == 0x64)
                {
                    KEY_DOWN = TRUE;
                    if (bol_hf)
                    {
                        FLASH_DATA_HF_STATUS_WRITE(TRUE);
                    }
                    else
                    {
                        FLASH_DATA_HF_STATUS_WRITE(FALSE);
                    }
                    HF_STATUS = bol_hf;
                    whileback = TRUE;
                }
                break;
            }
        }
        if (whileback)
        {
            syswhile = FALSE;
            break;
        }
    }
}