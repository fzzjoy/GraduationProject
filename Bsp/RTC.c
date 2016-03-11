#include "RTC.h"

/*******************************************************************************
* Function Name  : Time_ConvCalendarToUnix(struct tm t)
* Description    : ʱ���ʽת��
* Input 		 : struct tm t
* Output		 : None
* Return		 : u32
*******************************************************************************/
static u32 Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900;  //�ⲿtm�ṹ��洢�����Ϊ2008��ʽ
						//��time.h�ж������ݸ�ʽΪ1900�꿪ʼ�����
						//���ԣ�������ת��ʱҪ���ǵ�������ء�
	return mktime(&t);
}

/*******************************************************************************
* Function Name  : RTC_Configuration(void)
* Description    : ����RTC
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
static void RTC_Configuration(void)
{
	/* ʹ�� RTC �ͺ󱸼Ĵ������� */
	PWR_BackupAccessCmd(ENABLE);
	/* ��λ���ݼĴ������� */
	BKP_DeInit();
	/* ���� LSE */
//	RCC_LSEConfig(RCC_LSE_ON);
	RCC_LSICmd(ENABLE);
	/* �ȴ�LSE���� */
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	/* ѡ�� LSE Ϊ RTC ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	/* ʹ�� RTC ʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	/* �ȴ� RTC �Ĵ���ͬ����� */
	RTC_WaitForSynchro();
	/* �ȴ����һ�ζ� RTC �Ĵ�����д������� */
	RTC_WaitForLastTask();
	/* ʧ��RTC���ж� */  
	RTC_ITConfig(RTC_IT_SEC, DISABLE);
	/* �ȴ����һ�ζ� RTC �Ĵ�����д������� */
	RTC_WaitForLastTask();
	/* ����RTCʱ�ӷ�ƵֵΪ32767�������Ƶ�� = (32.768 KHz)/(32767+1) = 1Hz��~1s��*/
	RTC_SetPrescaler(32767); 
	/* �ȴ����һ�ζ� RTC �Ĵ�����д������� */
	RTC_WaitForLastTask();
}


/*******************************************************************************
* Function Name  : Time_GetRTCTimeByUnix()
* Description    : ��RTCȡ��ǰʱ���Unixʱ���ֵ
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
* Description    : ��������Calendar��ʽʱ��ת����UNIXʱ���д��RTC
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
* Description    : ��������Unixʱ���д��RTC
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
* Description    : ת��UNIXʱ���Ϊ����ʱ��
* Input 		 : u32 t  ��ǰʱ���UNIXʱ���
* Output		 : None
* Return		 : struct tm
*******************************************************************************/
struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	//localtimeת�������tm_year�����ֵ����Ҫת�ɾ���ֵ
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
 	Time_SetRTCTimeByCalendar(time_now); //�趨��ʼʱ��
 	BKP_WriteBackupRegister(BKP_DR1, 0x5A6A);
  }
  else
  {
	  /* ʹ�� RTC �ͺ󱸼Ĵ������� */
	  PWR_BackupAccessCmd(ENABLE);
	  /* ��λ���ݼĴ������� */
	//  BKP_DeInit();
	  /* ���� LSE */
  //  RCC_LSEConfig(RCC_LSE_ON);
	  RCC_LSICmd(ENABLE);
	  /* �ȴ�LSE���� */
	  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	  /* ѡ�� LSE Ϊ RTC ʱ��Դ */
	  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	  /* ʹ�� RTC ʱ�� */
	  RCC_RTCCLKCmd(ENABLE);
	  /* �ȴ� RTC �Ĵ���ͬ����� */
	  RTC_WaitForSynchro();
	  /* �ȴ����һ�ζ� RTC �Ĵ�����д������� */
	  RTC_WaitForLastTask();
	  /* ʧ��RTC���ж� */  
	  RTC_ITConfig(RTC_IT_SEC, DISABLE);
	  /* �ȴ����һ�ζ� RTC �Ĵ�����д������� */
	  RTC_WaitForLastTask();
	  /* ����RTCʱ�ӷ�ƵֵΪ32767�������Ƶ�� = (32.768 KHz)/(32767+1) = 1Hz��~1s��*/
	  RTC_SetPrescaler(32767); 
	  /* �ȴ����һ�ζ� RTC �Ĵ�����д������� */
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



