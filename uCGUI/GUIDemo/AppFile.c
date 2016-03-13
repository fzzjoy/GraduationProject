#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "AppFrame.h"
#include "AppCommon.h"

#include "stm32_eval_sdio_sd.h"
#include <stdio.h>
#include "ff.h"

#define BUTTON_ID_EXIT		0

static FILINFO fileinfo;
static char filenamebuff[20]={0};

static int i=0;
static char *s="/";
static U8 FirstInFlag=0;

static void _ShowButton(WM_HWIN hWin)
{
	BUTTON_Handle _ahButton;

	_ahButton = BUTTON_CreateAsChild(10, 10, 30, 25, hWin, BUTTON_ID_EXIT, WM_CF_SHOW | WM_CF_STAYONTOP | WM_CF_MEMDEV);
	BUTTON_SetText(_ahButton, "EXIT");
}

static void _cbFile(WM_MESSAGE *pMsg)
{
	int NCode, Id;
	switch (pMsg->MsgId)
	{
	case WM_PAINT:
/*		GUI_SetBkColor(GUI_WHITE);
		GUI_Clear();
		GUI_SetColor(GUI_RED);*/

		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
		NCode = pMsg->Data.v;            /* Notification code */
		switch (NCode) 
		{
		case WM_NOTIFICATION_CLICKED:
			switch (Id)
			{
			case BUTTON_ID_EXIT:
				GUI_SetAppIndex(APP_MainMenu);
				GUI_SetAppDirection(MEMDEV_ANIMATION_LEFT);
				_MoveShiftWindow(&pMsg->hWin, MEMDEV_ANIMATION_RIGHT, 1, 1);
				GUI_SetCancel(APP_File);
				GUI_ClearHalt();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	default:
		WM_DefaultProc(pMsg);
	}
}

u8 listFiles(char * path)
{
	FRESULT res;	  
	//char *fn;	/* This function is assuming non-Unicode cfg. */
	DIR dir;
 	char tmppath[20];
    strcpy(tmppath,path);
	
	res = f_opendir(&dir,(const TCHAR*)path); //
	if (res == FR_OK) 
	{	
		while(1)
		{
			res = f_readdir(&dir, &fileinfo);					
			if (res != FR_OK || fileinfo.fname[0] == 0) break;	
			if (FirstInFlag== 0) 
			{
				FirstInFlag=1;
				continue;			  
			}		   
			//fn = fileinfo.fname;
			sprintf(filenamebuff,"%s %s",path,fileinfo.fname);
			GUI_DispStringHCenterAt(filenamebuff, 160, i*20);
			i++;
			strcat(tmppath,s);
			strcat(tmppath,fileinfo.fname);
			GUI_DispStringHCenterAt(tmppath, 0, 0);
			listFiles(tmppath);
			strcpy(tmppath,path);
		} 
	}
	f_closedir(&dir);
	return res;   
}

void FileSDInit(void)
{
	SD_Error status;
	FRESULT res;
	FATFS fs;

	status = SD_Init();
	if(status != SD_OK)
	{
		GUI_DispStringHCenterAt("sd card init error.", 160, 20);
	}

	res = f_mount(&fs ,"0:", 0);
	if(res != FR_OK)
	{
		GUI_DispStringHCenterAt("f_mount error.", 160, 40);
	} 		
}

/*******************************************************************
*
*       MainTask
*
********************************************************************
*/
void MainTask_AppFile(void)
{
	WM_HWIN hWinFile;
	char path[20]="0:";

	hWinFile = WM_CreateWindow(320, 0, 320, 240, WM_CF_SHOW, _cbFile, 0);
	WM_SetFocus(hWinFile);
	_MoveShiftWindow(&hWinFile, MEMDEV_ANIMATION_LEFT, WM_Shift_ToLCD, 0);
	WM_EnableMemdev(hWinFile);

	GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
	GUI_SetColor(GUI_RED);
	GUI_SetFont(&GUI_Font16_ASCII);
	_ShowButton(hWinFile);
	
	FileSDInit();
//	listFiles(path);

	while (!GUI_CheckCancel(APP_File))
	{
		GUI_Delay(10);
	}
	WM_DeleteWindow(hWinFile);
	hWinFile = 0;
}


