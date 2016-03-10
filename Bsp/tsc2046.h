/******************** (C) COPYRIGHT 2011 baiweijishu ********************
* File Name          : tsc2046.h
* Author             : zhangxl
* Version            : V1.0
* Date               : 08/14/2011
* Description        : Header for tsc2046.c file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TSC2046_H
#define __TSC2046_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */
#if !defined (USE_STM3210B_EVAL) &&  !defined (USE_STM3210E_EVAL)
 //#define USE_STM3210B_EVAL
 #define USE_STM3210E_EVAL
#endif

#ifdef USE_STM3210B_EVAL
 #define GPIO_TP_CS                  GPIOA
 #define RCC_APB2Periph_GPIO_TP_CS   RCC_APB2Periph_GPIOA
 #define GPIO_Pin_TP_CS              GPIO_Pin_4 
#else /* USE_STM3210E_EVAL */
 #define GPIO_TP_CS                  GPIOF
 #define RCC_APB2Periph_GPIO_TP_CS   RCC_APB2Periph_GPIOF
 #define GPIO_Pin_TP_CS              GPIO_Pin_10 
#endif

/* Exported macro ------------------------------------------------------------*/
/* Select TP: Chip Select pin low  */
#define SPI_TOUCH_CS_LOW()       GPIO_ResetBits(GPIOF, GPIO_Pin_10)
/* Deselect TP: Chip Select pin high */
#define SPI_TOUCH_CS_HIGH()      GPIO_SetBits(GPIOF, GPIO_Pin_10)

/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
void SPI_TOUCH_Init(void);
u16 SPI_TOUCH_Read_X(void);
u16 SPI_TOUCH_Read_Y(void);

/*----- Low layer function -----*/
u8 SPI_TOUCH_ReadByte(void);
u8 SPI_TOUCH_SendByte(u8 byte);

void SPI_Delay(u32 count);
#endif /* __TSC2046_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
