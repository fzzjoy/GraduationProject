/**
*****************************��Ϊ�������޹�˾*********************************
���ߣ�������
ʱ�䣺2013��5��10��
����֧����̳��www.stm32bbs.com
�ٷ��Ա�����: http://baiweijishu.taobao.com
*******************************STM32������̳**********************************
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
