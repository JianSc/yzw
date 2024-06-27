/**
 ******************************************************************************
 * @file    Project/main.c
 * @author  MCD Application Team
 * @version V2.3.0
 * @date    16-June-2017
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "init.h"

/* Private defines -----------------------------------------------------------*/

// TODO:TM1650-LED码	   0	 1		2     3     4     5    6     7     8     9     A     B     C     D     E     F	  P 	-
// LED7codeH 共阳 , LED7codeL 共阴
uint8_t LED7CodeH[] = {~0x3f, ~0x06, ~0x5b, ~0x4f, ~0x66, ~0x6d, ~0x7d, ~0x07, ~0x7f, ~0x6f, ~0x77, ~0x7c, ~0x39, ~0x5e, ~0x79, ~0x71, ~0x73, ~0x40};
uint8_t LED7CodeL[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x73, 0x40};

/* Private function prototypes -----------------------------------------------*/
int TEM_SUM;
u8 FAN_PWM_NUM;
bool LED_STATUS, TEM_STATUS, CONFIG_STATUS;
bool SYSTEM_RUN, DS18B20_CONVERT_STATUS, HF_STATUS, MSD_STATUS, MSD_TIME_STATUS;
bool IT_HF_STATUS;
// btn[0] 为启动/停止按钮, btn[1] 设置按钮, btn[2] +按钮， btn[3] -按钮, btn[4] 确定/灯光按钮.
u8 btn[] = {0x44, 0x4c, 0x54, 0x5c, 0x64};

void main(void)
{
  /* Infinite loop */
  init();
  TIM2_Cmd(ENABLE);
  TIM1_Cmd(ENABLE);
  tm1650_displayInit();
  delay_ms(100);
  MSD_STATUS = FALSE;
  MSD_TIME_STATUS = FALSE;

  // 读取数据
  TEM_SUM = FLASH_DATA_TEM_SUM_READ();
  FAN_PWM_NUM = FLASH_DATA_FAN_PWM_READ();
  TEM_STATUS = FLASH_DATA_TEM_STATUS_READ();
  LED_STATUS = FLASH_DATA_LED_STATUS_READ();
  HF_STATUS = FLASH_DATA_HF_STATUS_READ();

  SYSTEM_RUN = TRUE;
  DS18B20_CONVERT_STATUS = FALSE;

  TEM_LED_CL;

  while (1)
  {
    // 过温检测，如果温度高于设定的5度，并且持续30秒，就关闭加温继电器
    // if(MSD_TIME_STATUS)
    // {
    //   MSD_CL;
    //   SW_CL;
    //   LED_STATUS = TRUE;
    //   FAN_CL;
    //   TEM_LED_CL;

    //   while(1);
    // }
    // 启动 DS18B20 进行温度转换
    int SUM;
    if (!DS18B20_CONVERT_STATUS)
    {
      disableInterrupts();

      // 转换温度
      while (!DS18B20_Init())
      {
        delay_ms(10);
      }
      ds18b20_Conver();
      DS18B20_CONVERT_STATUS = TRUE;

      // 读取温度
      while (!DS18B20_Init())
      {
        delay_ms(10);
      };
      SUM = DS18B20_Convert_Get_Tem(DS18B20_Read());
      delay_ms(5);

      enableInterrupts();

      u8 x;
      if (!SYSTEM_RUN && x > 2)
      {
        // 显示温度
        SUM = (SUM < -99) ? -99 : SUM;
        if (SUM < 0)
        {
          tm1650_displayW(0, 0x40);
          SUM = ~SUM + 1;
          tm1650_displayW(1, (LED7CodeL[SUM / 10 % 10]) | 0x80);
          tm1650_displayW(2, LED7CodeL[SUM % 10]);
        }
        else
        {
          SUM = (SUM > 999) ? 999 : SUM;
          if ((SUM / 100 % 100) == 0)
          {
            tm1650_displayW(0, 0x00);
          }
          else
          {
            tm1650_displayW(0, LED7CodeL[SUM / 100 % 100]);
          }
          tm1650_displayW(1, (LED7CodeL[SUM / 10 % 10]) | 0x80);
          tm1650_displayW(2, LED7CodeL[SUM % 10]);
        }
      }
      x++;
      x = (x > 10) ? 0 : x;
    }

    // LED 开启状态
    if (LED_STATUS)
    {
      LED_LED_EN;
    }
    else
    {
      LED_LED_CL;
    }

    // 开启加温
    if (TEM_STATUS)
    {
      if (SYSTEM_RUN)
      {
        delay_ms(500);
      }
      else
      {
        if (SUM < TEM_SUM)
        {
          SW_EN;
          TEM_LED_EN;
        }
        else
        {
          SW_CL;
          TEM_LED_CL;
        }
      }
      FAN_LED_EN;
      TIM2_SetCompare3(FAN_PWM_NUM);
    }
    else
    {
      SW_CL;
      TEM_LED_CL;
      FAN_LED_CL;
      TIM2_SetCompare3(0);
    }

    SYSTEM_RUN = FALSE;

    // 换风
    if (HF_STATUS)
    {
      if ((SUM - TEM_SUM) > 10 && !IT_HF_STATUS)
      {
        IT_HF_STATUS = TRUE;
      }
      if (IT_HF_STATUS)
      {
        HF_EN;
      }
      else
      {
        HF_CL;
      }
    }

    // 匹配按键
    u8 KEY = tm1650_displaykeyR();
    btn_down(KEY);
  }
}

void assert_failed(u8 *file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  // while (1)
  // {
  // }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
