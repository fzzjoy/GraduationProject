/**
*****************************百为技术有限公司*********************************
作者：张锡良
时间：2013年10月10日
技术支持论坛；www.stm32bbs.com
官方淘宝网店: http://baiweijishu.taobao.com
*******************************STM32技术论坛**********************************
**/  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32_eval.h"
#include <stdio.h>
#include "stm3210e_eval_lcd.h" 
#include "stm32_eval_sdio_sd.h"
#include <ucos_ii.h>
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "ff.h"
#include "RTC.h"
#include "time.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   " STM32 High Density " 
#define MESSAGE2   " Device running on  " 
#define MESSAGE3   "   STM3210E-EVAL    " 


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;

FATFS fs;
FILINFO fno;
DIR dir;
FIL F;
FRESULT res;
uint32_t BytesWritten;
uint32_t BytesRead;
uint8_t Buffer[_MAX_SS];

const uint8_t BmpCheck[2] =
  {
    0x42, 0x4D
  };

struct tm CurTime; 


/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/
void CheckBitmapFiles(void);
uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     

 /*Init RTC*/
 /*使能时钟线*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  CurTime.tm_year = 2016;
  CurTime.tm_mon =  3;
  CurTime.tm_mday = 11;
  CurTime.tm_hour = 22;
  CurTime.tm_min = 52;
  CurTime.tm_sec = 40;
  CurTime.tm_wday=5;
  RTC_Init(CurTime);

  /* Initialize LEDs, Key Button, LCD and COM port(USART) available on
     STM3210X-EVAL board ******************************************************/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);

  /* Initialize the LCD */
  STM3210E_LCD_Init();

	SPI_TOUCH_Init();

  /* Display message on STM3210X-EVAL LCD *************************************/
  /* Clear the LCD */ 
//  LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Back Color */
//  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the LCD Text Color */
//  LCD_SetTextColor(LCD_COLOR_WHITE);
//  LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)MESSAGE1);
//  LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)MESSAGE2);
//  LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)MESSAGE3);

  /* Retarget the C library printf function to the USARTx, can be USART1 or USART2
     depending on the EVAL board you are using ********************************/
  printf("\n\r %s", MESSAGE1);
  printf(" %s", MESSAGE2);
  printf(" %s\n\r", MESSAGE3);

  /* Turn on leds available on STM3210X-EVAL **********************************/
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);

  /* Add your application code here
     */
// 	CheckBitmapFiles();
 	App_TaskStart();

  /* Infinite loop */
  while (1)
  {
  }
}

void CheckBitmapFiles(void)
{
	SD_Error status;
	status = SD_Init();
	if(status != SD_OK)
	{
		printf("sd card init error.\n\r");
	}

	res = f_mount(&fs ,"0:", 0);
	if(res != FR_OK)
	{
		printf("f_mount error = %d\n\r",res);
	}

	if (f_open (&F, "STFILES/STLOGO.BMP", FA_READ))
	{
		printf("f_open file error.\n\r");
	}

	f_read (&F, Buffer, _MAX_SS, &BytesRead);

	if (Buffercmp((uint8_t *)BmpCheck,  Buffer, 2) != 0)
	{
	  /* Set the Back Color */
	  LCD_SetBackColor(LCD_COLOR_BLUE);
	  /* Set the Text Color */
	  LCD_SetTextColor(LCD_COLOR_WHITE); 
	
	  LCD_Clear(White);
	  /* Display the "  Plug the USB   " message */	 
	  LCD_DisplayStringLine(Line7, " No Bmp Image Found ");
	  LCD_DisplayStringLine(Line8, " Plug the USB Cable ");
	  LCD_DisplayStringLine(Line9, "Copy File To Storage");
	
	  Set_System();
	  Set_USBClock();
	  USB_Interrupts_Config();
	  USB_Init();
	  while (bDeviceState != CONFIGURED)
		{
		}
	
		LCD_Clear(White);
	  LCD_ClearLine(Line9);
	  /* Display the "To stop Press SEL" message */	
	  LCD_DisplayStringLine(Line7, " After Copying File ");
	  LCD_DisplayStringLine(Line8, "   Restart System   ");
	
	  USB_Configured_LED(); 
		while(1)
		{
		}     
	}
	else
	{
		f_close (&F);
	}
}

/*******************************************************************************
* Function Name  : Buffercmp
* Description    : Compares two buffers.
* Input          : - pBuffer1, pBuffer2: buffers to be compared.
*                : - BufferLength: buffer's length
* Output         : None
* Return         : 0: pBuffer1 identical to pBuffer2
*                  1: pBuffer1 differs from pBuffer2
*******************************************************************************/
uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
