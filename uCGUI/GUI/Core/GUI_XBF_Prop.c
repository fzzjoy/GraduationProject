/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUI_XBF_Prop.c
Purpose     : Implementation of system independend fonts
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Private.h"
#include "ff.h"
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIPROP_FindChar
*/
static const GUI_FONT_PROP GUI_UNI_PTR * GUIPROP_FindChar(const GUI_FONT_PROP GUI_UNI_PTR* pProp, U16P c) {
//	printf("%s\n\r",__FUNCTION__);
  for (; pProp; pProp = pProp->pNext) {
    if ((c>=pProp->First) && (c<=pProp->Last))
      break;
  }
  return pProp;
}


/*********************************************************************
*
*       _GetpCharData
*/
static const U8 * _GetpCharData(const GUI_FONT_PROP GUI_UNI_PTR * pProp, U16P c, U8 * pData) {
//  const U8 * pCharArea, * pCharInfo;
//  int NumCharAreas;
//  NumCharAreas = _GetNumCharAreas(pFont);                 /* Get # of char areas */
//  pCharArea = (const U8 *)pFont->p.pFontData 
//              + sizeof(GUI_SI_FONT);                      /* Set char area pointer to first char area */
//  pCharInfo = pCharArea
//              + sizeof(GUI_XBF_CHAR_AREA) * NumCharAreas; /* Set char info pointer to first character info */
//  do {
//    GUI_XBF_CHAR_AREA CharArea;
//    CharArea.First    = GUI__Read16(&pCharArea);
//    CharArea.Last     = GUI__Read16(&pCharArea);
//    if ((c >= CharArea.First) && (c <= CharArea.Last)) {
//      pCharInfo += sizeof(GUI_XBF_CHARINFO) * (c - CharArea.First);
//      return pCharInfo;
//    }
//    pCharInfo += sizeof(GUI_XBF_CHARINFO) * (CharArea.Last - CharArea.First + 1);
//  } while(--NumCharAreas);
//  return 0;
	FIL fl;
	U32 len;
	U32 offset;
 	U8 byteOfDots;
	U8 *fileName;
	const GUI_CHARINFO GUI_UNI_PTR * pCharInfo = pProp->paCharInfo;
	byteOfDots = GUI_Context.pAFont->YSize * pCharInfo->BytesPerLine; 
	fileName = (char *)pCharInfo->pData;

	if (c < 0xA1)                                                              
	{
		offset = c * byteOfDots;
	}
	else                                                                          
	{
		offset = 94 * ((c & 0xFF) - 0xA1) * byteOfDots + ((c >> 8) - 0xA1) * byteOfDots;                                          
	}
//	printf("%s c=0x%x\n\r",__FUNCTION__,c);
 	f_open (&fl, fileName, FA_READ);
	f_lseek(&fl, offset);
	f_read (&fl, pData, byteOfDots, &len);
//	printf("%s fileName = %s\n\r",__FUNCTION__,fileName);
	
	f_close(&fl);

	return 0;
}

/*********************************************************************
*
*       Exported code
*
**********************************************************************
*/

/*********************************************************************
*
*       GUI_XBF_DispChar
*/
void GUIPROP_XBF_DispChar(U16P c) {
  U8 Data[128];
  int BytesPerLine;
  GUI_DRAWMODE DrawMode = GUI_Context.TextMode;
  const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUIPROP_FindChar(GUI_Context.pAFont->p.pProp, c);
//	printf("%s\n\r",__FUNCTION__);
  if (pProp) {
    GUI_DRAWMODE OldDrawMode;
    const GUI_CHARINFO GUI_UNI_PTR * pCharInfo = pProp->paCharInfo;
    BytesPerLine = pCharInfo->BytesPerLine;
    _GetpCharData(pProp, c, Data);
    OldDrawMode  = LCD_SetDrawMode(DrawMode);
    LCD_DrawBitmap(GUI_Context.DispPosX, GUI_Context.DispPosY,
                       pCharInfo->XSize,
											 GUI_Context.pAFont->YSize,
                       GUI_Context.pAFont->XMag,
											 GUI_Context.pAFont->YMag,
                       1,     /* Bits per Pixel */
                       BytesPerLine,
                       Data,
                       &LCD_BKCOLORINDEX
                       );
    /* Fill empty pixel lines */
    if (GUI_Context.pAFont->YDist > GUI_Context.pAFont->YSize) {
      int YMag = GUI_Context.pAFont->YMag;
      int YDist = GUI_Context.pAFont->YDist * YMag;
      int YSize = GUI_Context.pAFont->YSize * YMag;
      if (DrawMode != LCD_DRAWMODE_TRANS) {
        LCD_COLOR OldColor = GUI_GetColor();
        GUI_SetColor(GUI_GetBkColor());
        LCD_FillRect(GUI_Context.DispPosX, 
                     GUI_Context.DispPosY + YSize, 
                     GUI_Context.DispPosX + pCharInfo->XSize, 
                     GUI_Context.DispPosY + YDist);
        GUI_SetColor(OldColor);
      }
    }
    LCD_SetDrawMode(OldDrawMode); /* Restore draw mode */
    GUI_Context.DispPosX += pCharInfo->XDist * GUI_Context.pAFont->XMag;
  }
}

/*********************************************************************
*
*       GUI_XBF_GetCharDistX
*/
int GUIPROP_XBF_GetCharDistX(U16P c) {
  const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUIPROP_FindChar(GUI_Context.pAFont->p.pProp, c);
  return (pProp) ? (pProp->paCharInfo)->XSize * GUI_Context.pAFont->XMag : 0;
}

/*********************************************************************
*
*       GUI_XBF_GetFontInfo
*/
void GUIPROP_XBF_GetFontInfo(const GUI_FONT GUI_UNI_PTR * pFont, GUI_FONTINFO * pfi) {
  GUI_USE_PARA(pFont);
  pfi->Flags = GUI_FONTINFO_FLAG_PROP;
}

/*********************************************************************
*
*       GUI_XBF_IsInFont
*/
char GUIPROP_XBF_IsInFont(const GUI_FONT GUI_UNI_PTR * pFont, U16 c) {
  const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUIPROP_FindChar(pFont->p.pProp, c);
  return (pProp==NULL) ? 0 : 1;
}

///*********************************************************************
//*
//*       GUI_XBF_TYPE_PROP
//*/
//const tGUI_XBF_APIList GUI_XBF_APIList_Prop = {
//  _GUI_XBF_DispChar,
//  _GUI_XBF_GetCharDistX,
//  _GUI_XBF_GetFontInfo,
//  _GUI_XBF_IsInFont
//};
