/**
*****************************百为技术有限公司*********************************
作者：张锡良
时间：2013年5月10日
技术支持论坛；www.stm32bbs.com
官方淘宝网店: http://baiweijishu.taobao.com
*******************************STM32技术论坛**********************************
**/
/* Includes ------------------------------------------------------------------*/
#include "..\core\GUI.H"
#ifndef GUI_FLASH
   #define GUI_FLASH
#endif
extern GUI_FLASH const GUI_FONT GUI_FontHZ16;

GUI_FLASH   const GUI_CHARINFO GUI_FontHZ16_CharInfo[] = {
    {  8, 8,  1, (void GUI_FLASH *)"0:/font/ASC16" }, 
    {  16, 16, 2, (void GUI_FLASH *)"0:/font/HZK16" }, 
};

GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ16_PropHZ= {
      0xa1a1, 
      0xf7fe, 
      &GUI_FontHZ16_CharInfo[1], 
      (void *)0, 
};
GUI_FLASH const GUI_FONT_PROP GUI_FontHZ16_PropASC= {
      0x0020, 
      0x007f, 
      &GUI_FontHZ16_CharInfo[0], 
      (void GUI_FLASH *)&GUI_FontHZ16_PropHZ , 
};
GUI_FLASH const GUI_FONT GUI_FontHZ16 = {
      GUI_FONTTYPE_PROP_XBF, 
      16, 
      16, 
      1,  
      1,  
      (void GUI_FLASH *)&GUI_FontHZ16_PropASC
};
