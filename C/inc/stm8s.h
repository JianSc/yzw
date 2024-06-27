#ifndef _STM8S_H
#define _STM8S_H

/*!< Signed integer types  */
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed long int32_t;

/*!< Unsigned integer types  */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

/*!< STM8 Standard Peripheral Library old types (maintained for legacy purpose) */

typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#define U8_MAX  (255)
#define S8_MAX  (127)
#define S8_MIN  (-128)
#define U16_MAX (65535u)
#define S16_MAX (32767)
#define S16_MIN (-32768)
#define U32_MAX (4294967295uL)
#define S32_MAX (2147483647)
#define S32_MIN (-2147483648uL)

// IAR
#include "meintrins.h"
#define enableInterrupts()  __enable_interrupt()  /* enable interrupts */
#define disableInterrupts() __disable_interrupt() /* disable interrupts */
#define rim()               __enable_interrupt()  /* enable interrupts */
#define sim()               __disable_interrupt() /* disable interrupts */
// #define nop()               __no_operation()       /* No Operation */
#define nop    asm("nop")
#define trap() __trap()               /* Trap (soft IT) */
#define wfi()  __wait_for_interrupt() /* Wait For Interrupt */
#define halt() __halt()               /* Halt */

// typedef enum {
//     FALSE = 0,
//     TRUE  = !FALSE
// } bool;
#define bool  char
#define FALSE 0
#define TRUE  1

typedef enum {
    RESET = 0,
    SET   = !RESET
} FlagStatus, ITStatus, BitStatus, BitAction;

typedef enum {
    DISABLE = 0,
    ENABLE  = !DISABLE
} FunctionalState;

#define __I               volatile const /*!< defines 'read only' permissions     */
#define __O               volatile       /*!< defines 'write only' permissions    */
#define __IO              volatile       /*!< defines 'read / write' permissions  */

#define OPT_BaseAddress   0x4800
#define GPIOA_BaseAddress 0x5000
#define GPIOB_BaseAddress 0x5005
#define GPIOC_BaseAddress 0x500A
#define GPIOD_BaseAddress 0x500F
#define GPIOE_BaseAddress 0x5014
#define GPIOF_BaseAddress 0x5019
#define GPIOG_BaseAddress 0x501E
#define GPIOH_BaseAddress 0x5023
#define GPIOI_BaseAddress 0x5028
#define FLASH_BaseAddress 0x505A
#define EXTI_BaseAddress  0x50A0
#define RST_BaseAddress   0x50B3
#define CLK_BaseAddress   0x50C0
#define WWDG_BaseAddress  0x50D1
#define IWDG_BaseAddress  0x50E0
#define AWU_BaseAddress   0x50F0
#define BEEP_BaseAddress  0x50F3
#define SPI_BaseAddress   0x5200
#define I2C_BaseAddress   0x5210
#define UART1_BaseAddress 0x5230
#define UART2_BaseAddress 0x5240
#define UART3_BaseAddress 0x5240
#define UART4_BaseAddress 0x5230
#define TIM1_BaseAddress  0x5250
#define TIM2_BaseAddress  0x5300
#define TIM3_BaseAddress  0x5320
#define TIM4_BaseAddress  0x5340
#define TIM5_BaseAddress  0x5300
#define TIM6_BaseAddress  0x5340
#define ADC1_BaseAddress  0x53E0
#define ADC2_BaseAddress  0x5400
#define CAN_BaseAddress   0x5420
#define CFG_BaseAddress   0x7F60
#define ITC_BaseAddress   0x7F70
#define DM_BaseAddress    0x7F90

typedef struct ADC1_struct {
    __IO uint8_t DB0RH;   /*!< ADC1 Data Buffer Register (MSB)  */
    __IO uint8_t DB0RL;   /*!< ADC1 Data Buffer Register (LSB)  */
    __IO uint8_t DB1RH;   /*!< ADC1 Data Buffer Register (MSB)  */
    __IO uint8_t DB1RL;   /*!< ADC1 Data Buffer Register (LSB)  */
    __IO uint8_t DB2RH;   /*!< ADC1 Data Buffer Register (MSB)  */
    __IO uint8_t DB2RL;   /*!< ADC1 Data Buffer Register (LSB)  */
    __IO uint8_t DB3RH;   /*!< ADC1 Data Buffer Register (MSB)  */
    __IO uint8_t DB3RL;   /*!< ADC1 Data Buffer Register (LSB)  */
    __IO uint8_t DB4RH;   /*!< ADC1 Data Buffer Register (MSB)  */
    __IO uint8_t DB4RL;   /*!< ADC1 Data Buffer Register (LSB)  */
    __IO uint8_t DB5RH;   /*!< ADC1 Data Buffer Register (MSB)  */
    __IO uint8_t DB5RL;   /*!< ADC1 Data Buffer Register (LSB)  */
    __IO uint8_t DB6RH;   /*!< ADC1 Data Buffer Register (MSB)  */
    __IO uint8_t DB6RL;   /*!< ADC1 Data Buffer Register (LSB)  */
    __IO uint8_t DB7RH;   /*!< ADC1 Data Buffer Register (MSB)  */
    __IO uint8_t DB7RL;   /*!< ADC1 Data Buffer Register (LSB)  */
    __IO uint8_t DB8RH;   /*!< ADC1 Data Buffer Register (MSB)  */
    __IO uint8_t DB8RL;   /*!< ADC1 Data Buffer Register (LSB)  */
    __IO uint8_t DB9RH;   /*!< ADC1 Data Buffer Register (MSB)  */
    __IO uint8_t DB9RL;   /*!< ADC1 Data Buffer Register (LSB)  */
    uint8_t RESERVED[12]; /*!< Reserved byte */
    __IO uint8_t CSR;     /*!< ADC1 control status register */
    __IO uint8_t CR1;     /*!< ADC1 configuration register 1 */
    __IO uint8_t CR2;     /*!< ADC1 configuration register 2 */
    __IO uint8_t CR3;     /*!< ADC1 configuration register 3  */
    __IO uint8_t DRH;     /*!< ADC1 Data high */
    __IO uint8_t DRL;     /*!< ADC1 Data low */
    __IO uint8_t TDRH;    /*!< ADC1 Schmitt trigger disable register high */
    __IO uint8_t TDRL;    /*!< ADC1 Schmitt trigger disable register low */
    __IO uint8_t HTRH;    /*!< ADC1 high threshold register High*/
    __IO uint8_t HTRL;    /*!< ADC1 high threshold register Low*/
    __IO uint8_t LTRH;    /*!< ADC1 low threshold register high */
    __IO uint8_t LTRL;    /*!< ADC1 low threshold register low */
    __IO uint8_t AWSRH;   /*!< ADC1 watchdog status register high */
    __IO uint8_t AWSRL;   /*!< ADC1 watchdog status register low */
    __IO uint8_t AWCRH;   /*!< ADC1 watchdog control register high */
    __IO uint8_t AWCRL;   /*!< ADC1 watchdog control register low */
} ADC1_TypeDef;

typedef struct AWU_struct {
    __IO uint8_t CSR; /*!< AWU Control status register */
    __IO uint8_t APR; /*!< AWU Asynchronous prescaler buffer */
    __IO uint8_t TBR; /*!< AWU Time base selection register */
} AWU_TypeDef;

typedef struct BEEP_struct {
    __IO uint8_t CSR; /*!< BEEP Control status register */
} BEEP_TypeDef;

typedef struct CLK_struct {
    __IO uint8_t ICKR;     /*!< Internal Clocks Control Register */
    __IO uint8_t ECKR;     /*!< External Clocks Control Register */
    uint8_t RESERVED;      /*!< Reserved byte */
    __IO uint8_t CMSR;     /*!< Clock Master Status Register */
    __IO uint8_t SWR;      /*!< Clock Master Switch Register */
    __IO uint8_t SWCR;     /*!< Switch Control Register */
    __IO uint8_t CKDIVR;   /*!< Clock Divider Register */
    __IO uint8_t PCKENR1;  /*!< Peripheral Clock Gating Register 1 */
    __IO uint8_t CSSR;     /*!< Clock Security System Register */
    __IO uint8_t CCOR;     /*!< Configurable Clock Output Register */
    __IO uint8_t PCKENR2;  /*!< Peripheral Clock Gating Register 2 */
    uint8_t RESERVED1;     /*!< Reserved byte */
    __IO uint8_t HSITRIMR; /*!< HSI Calibration Trimmer Register */
    __IO uint8_t SWIMCCR;  /*!< SWIM clock control register */
} CLK_TypeDef;

typedef struct EXTI_struct {
    __IO uint8_t CR1; /*!< External Interrupt Control Register for PORTA to PORTD */
    __IO uint8_t CR2; /*!< External Interrupt Control Register for PORTE and TLI */
} EXTI_TypeDef;

typedef struct FLASH_struct {
    __IO uint8_t CR1;   /*!< Flash control register 1 */
    __IO uint8_t CR2;   /*!< Flash control register 2 */
    __IO uint8_t NCR2;  /*!< Flash complementary control register 2 */
    __IO uint8_t FPR;   /*!< Flash protection register */
    __IO uint8_t NFPR;  /*!< Flash complementary protection register */
    __IO uint8_t IAPSR; /*!< Flash in-application programming status register */
    uint8_t RESERVED1;  /*!< Reserved byte */
    uint8_t RESERVED2;  /*!< Reserved byte */
    __IO uint8_t PUKR;  /*!< Flash program memory unprotection register */
    uint8_t RESERVED3;  /*!< Reserved byte */
    __IO uint8_t DUKR;  /*!< Data EEPROM unprotection register */
} FLASH_TypeDef;

typedef struct OPT_struct {
    __IO uint8_t OPT0;  /*!< Option byte 0: Read-out protection (not accessible in IAP mode) */
    __IO uint8_t OPT1;  /*!< Option byte 1: User boot code */
    __IO uint8_t NOPT1; /*!< Complementary Option byte 1 */
    __IO uint8_t OPT2;  /*!< Option byte 2: Alternate function remapping */
    __IO uint8_t NOPT2; /*!< Complementary Option byte 2 */
    __IO uint8_t OPT3;  /*!< Option byte 3: Watchdog option */
    __IO uint8_t NOPT3; /*!< Complementary Option byte 3 */
    __IO uint8_t OPT4;  /*!< Option byte 4: Clock option */
    __IO uint8_t NOPT4; /*!< Complementary Option byte 4 */
    __IO uint8_t OPT5;  /*!< Option byte 5: HSE clock startup */
    __IO uint8_t NOPT5; /*!< Complementary Option byte 5 */
    uint8_t RESERVED1;  /*!< Reserved Option byte*/
    uint8_t RESERVED2;  /*!< Reserved Option byte*/
    __IO uint8_t OPT7;  /*!< Option byte 7: flash wait states */
    __IO uint8_t NOPT7; /*!< Complementary Option byte 7 */
} OPT_TypeDef;

typedef struct GPIO_struct {
    __IO uint8_t ODR; /*!< Output Data Register */
    __IO uint8_t IDR; /*!< Input Data Register */
    __IO uint8_t DDR; /*!< Data Direction Register */
    __IO uint8_t CR1; /*!< Configuration Register 1 */
    __IO uint8_t CR2; /*!< Configuration Register 2 */
} GPIO_TypeDef;

#define ADC1  ((ADC1_TypeDef *)ADC1_BaseAddress)
#define AWU   ((AWU_TypeDef *)AWU_BaseAddress)
#define BEEP  ((BEEP_TypeDef *)BEEP_BaseAddress)
#define CLK   ((CLK_TypeDef *)CLK_BaseAddress)
#define EXTI  ((EXTI_TypeDef *)EXTI_BaseAddress)
#define FLASH ((FLASH_TypeDef *)FLASH_BaseAddress)
#define OPT   ((OPT_TypeDef *)OPT_BaseAddress)
#define GPIOA ((GPIO_TypeDef *)GPIOA_BaseAddress)
#define GPIOB ((GPIO_TypeDef *)GPIOB_BaseAddress)
#define GPIOC ((GPIO_TypeDef *)GPIOC_BaseAddress)
#define GPIOD ((GPIO_TypeDef *)GPIOD_BaseAddress)
#define GPIOE ((GPIO_TypeDef *)GPIOE_BaseAddress)
#define GPIOF ((GPIO_TypeDef *)GPIOF_BaseAddress)

#endif