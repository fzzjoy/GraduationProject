/**
*****************************百为技术有限公司*********************************
作者：张锡良
时间：2013年10月10日
技术支持论坛；www.stm32bbs.com
官方淘宝网店: http://baiweijishu.taobao.com
*******************************STM32技术论坛**********************************
**/  
#include "stm32f10x.h"
#include "stm32_eval.h"
#include <ucos_ii.h>

#define User_Task_PRIO       15
#define Task2_PRIO       6
#define Task3_PRIO       7

#define USE_TASK_STACK_SIZE 512
#define TASK2_STACK_SIZE 		128
#define TASK3_STACK_SIZE 		128

static	OS_STK	User_Task_Stack[USE_TASK_STACK_SIZE];
static	OS_STK	Task2_Stack[TASK2_STACK_SIZE];
static	OS_STK	Task3_Stack[TASK3_STACK_SIZE];

void User_Task_Func(void *p_arg)
{
	(void)p_arg;
	while(1)
	{
//		printf("user task start.\n\r");
		MainTask();
	}
}


void Task2_Func(void *p_arg)
{
	(void)p_arg;
	while(1)
	{
		GUI_TOUCH_Exec();
//		printf("**********task2\n\r");
		OSTimeDly(20);
	}
}

/*void Task3_Func(void *p_arg)
{
	(void)p_arg;
	while(1)
	{
//		printf("********************task3\n\r");
		STM_EVAL_LEDOn(LED3);
		OSTimeDly(1000); 
		STM_EVAL_LEDOff(LED3);
		OSTimeDly(1000);
	}
} */


void App_TaskStart(void)
{
	OSInit();
	SysTick_Config(SystemCoreClock / OS_TICKS_PER_SEC);
	OSTaskCreate(User_Task_Func, (void *)0, (OS_STK *)&User_Task_Stack[USE_TASK_STACK_SIZE-1],  User_Task_PRIO);
	OSTaskCreate(Task2_Func, (void *)0, (OS_STK *)&Task2_Stack[TASK2_STACK_SIZE-1],  Task2_PRIO);
//	OSTaskCreate(Task3_Func, (void *)0, (OS_STK *)&Task3_Stack[TASK3_STACK_SIZE-1],  Task3_PRIO);
	OSStart();
}
