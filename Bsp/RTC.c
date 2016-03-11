#include "RTC.h"

/*******************************************************************************
* Function Name  : Time_ConvCalendarToUnix(struct tm t)
* Description    : 时间格式转换
* Input 		 : struct tm t
* Output		 : None
* Return		 : u32
*******************************************************************************/
static u32 Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900;  //外部tm结构体存储的年份为2008格式
						//而time.h中定义的年份格式为1900年开始的年份
						//所以，在日期转换时要考虑到这个因素。
	return mktime(&t);
}

/*******************************************************************************
* Function Name  : RTC_Configuration(void)
* Description    : 配置RTC
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
static void RTC_Configuration(void)
{
	/* 使能 RTC 和后备寄存器访问 */
	PWR_BackupAccessCmd(ENABLE);
	/* 复位备份寄存器设置 */
	BKP_DeInit();
	/* 开启 LSE */
//	RCC_LSEConfig(RCC_LSE_ON);
	RCC_LSICmd(ENABLE);
	/* 等待LSE起振 */
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	/* 选择 LSE 为 RTC 时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	/* 使能 RTC 时钟 */
	RCC_RTCCLKCmd(ENABLE);
	/* 等待 RTC 寄存器同步完成 */
	RTC_WaitForSynchro();
	/* 等待最近一次对 RTC 寄存器的写操作完成 */
	RTC_WaitForLastTask();
	/* 失能RTC秒中断 */  
	RTC_ITConfig(RTC_IT_SEC, DISABLE);
	/* 等待最近一次对 RTC 寄存器的写操作完成 */
	RTC_WaitForLastTask();
	/* 设置RTC时钟分频值为32767，则计数频率 = (32.768 KHz)/(32767+1) = 1Hz（~1s）*/
	RTC_SetPrescaler(32767); 
	/* 等待最近一次对 RTC 寄存器的写操作完成 */
	RTC_WaitForLastTask();
}


/*******************************************************************************
* Function Name  : Time_GetRTCTimeByUnix()
* Description    : 从RTC取当前时间的Unix时间戳值
* Input 		 : None
* Output		 : None
* Return		 : u32 t
*******************************************************************************/
u32 Time_GetRTCTimeByUnix(void)
{
	return (u32)RTC_GetCounter();
}

/*******************************************************************************
* Function Name  : Time_SetRTCTimeByCalendar()
* Description    : 将给定的Calendar格式时间转换成UNIX时间戳写入RTC
* Input 		 : struct tm t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetRTCTimeByCalendar(struct tm t)
{
	Time_SetRTCTimeByUnix(Time_ConvCalendarToUnix(t));
	return;
}


/*******************************************************************************
* Function Name  : Time_SetRTCTimeByUnix()
* Description    : 将给定的Unix时间戳写入RTC
* Input 		 : time_t t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetRTCTimeByUnix(time_t t)
{
	RTC_WaitForLastTask();
	RTC_SetCounter((u32)t);
	RTC_WaitForLastTask();
	return;
}

/*******************************************************************************
* Function Name  : Time_ConvUnixToCalendar(time_t t)
* Description    : 转换UNIX时间戳为日历时间
* Input 		 : u32 t  当前时间的UNIX时间戳
* Output		 : None
* Return		 : struct tm
*******************************************************************************/
struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	//localtime转换结果的tm_year是相对值，需要转成绝对值
	return *t_tm;
}


/*******************************************************************************
* Function Name	: RTC_Init()
* Description	: RTC init
* Input			: None
* Output 		: None
* Return 		: None
*******************************************************************************/
void RTC_Init(struct tm time_now)
{
//	RTC_Configuration();
//	Time_SetRTCTimeByCalendar(time_now);
  if (BKP_ReadBackupRegister(BKP_DR1) != 0x5A6A)
  {
	RTC_Configuration();
 	Time_SetRTCTimeByCalendar(time_now); //设定初始时间
 	BKP_WriteBackupRegister(BKP_DR1, 0x5A6A);
  }
  else
  {
	  /* 使能 RTC 和后备寄存器访问 */
	  PWR_BackupAccessCmd(ENABLE);
	  /* 复位备份寄存器设置 */
	//  BKP_DeInit();
	  /* 开启 LSE */
  //  RCC_LSEConfig(RCC_LSE_ON);
	  RCC_LSICmd(ENABLE);
	  /* 等待LSE起振 */
	  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	  /* 选择 LSE 为 RTC 时钟源 */
	  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	  /* 使能 RTC 时钟 */
	  RCC_RTCCLKCmd(ENABLE);
	  /* 等待 RTC 寄存器同步完成 */
	  RTC_WaitForSynchro();
	  /* 等待最近一次对 RTC 寄存器的写操作完成 */
	  RTC_WaitForLastTask();
	  /* 失能RTC秒中断 */  
	  RTC_ITConfig(RTC_IT_SEC, DISABLE);
	  /* 等待最近一次对 RTC 寄存器的写操作完成 */
	  RTC_WaitForLastTask();
	  /* 设置RTC时钟分频值为32767，则计数频率 = (32.768 KHz)/(32767+1) = 1Hz（~1s）*/
	  RTC_SetPrescaler(32767); 
	  /* 等待最近一次对 RTC 寄存器的写操作完成 */
	  RTC_WaitForLastTask();

  }
	/* Adjust time by values entred by the user on the hyperterminal */
 /*  BKP_WriteBackupRegister(BKP_DR1, 0x5A6A);
  }
  else
  {
	  ;
	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
	  printf("\r\n\n Power On Reset occurred....");
 
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
	{
	  printf("\r\n\n External Reset occurred....");
 
	}*/
	/* Wait for RTC registers synchronization */
//	RTC_WaitForSynchro();
	 /* Enable the RTC Second */
//	RTC_ITConfig(RTC_IT_SEC, DISABLE);
	/* Wait until last write operation on RTC registers has finished */
//	RTC_WaitForLastTask();
//  }
}



