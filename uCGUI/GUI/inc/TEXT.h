/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              �C/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : TEXT.h
Purpose     : TEXT include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef TEXT_H
#define TEXT_H

#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"      /* Req. for Create indirect data structure */
#include "GUIDebug.h" /* Req. for GUI_DEBUG_LEVEL */

#if GUI_WINSUPPORT

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/************************************************************
*
*       #defines
*
*************************************************************
*/

/************************************************************
*
*       Create / Status flags
*/
#define TEXT_CF_LEFT    GUI_TA_LEFT
#define TEXT_CF_RIGHT   GUI_TA_RIGHT
#define TEXT_CF_HCENTER GUI_TA_HCENTER

#define TEXT_CF_VCENTER GUI_TA_VCENTER
#define TEXT_CF_TOP     GUI_TA_TOP
#define TEXT_CF_BOTTOM  GUI_TA_BOTTOM

/************************************************************
*
*       States
*/

#define TEXT_STATE_FOCUS      WIDGET_STATE_FOCUS
#define TEXT_STATE_PRESSED    WIDGET_STATE_USER0

/*********************************************************************
*
*       Public Types
*
**********************************************************************

*/
typedef WM_HMEM TEXT_Handle;

/*********************************************************************
*
*       Standard member functions
*
**********************************************************************
*/

#define TEXT_EnableMemdev(hObj)  WM_EnableMemdev(hObj)
#define TEXT_DisableMemdev(hObj) WM_DisableMemdev(hObj)
#define TEXT_Delete(hObj)        WM_DeleteWindow(hObj)
#define TEXT_Paint(hObj)         WM_Paint(hObj)
#define TEXT_Invalidate(hObj)    WM_InvalidateWindow(hObj)

/*********************************************************************
*
*       Create functions
*
**********************************************************************
*/

TEXT_Handle TEXT_Create        (int x0, int y0, int xsize, int ysize, int Id, int Flags, const char * s, int Align);
TEXT_Handle TEXT_CreateAsChild (int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags, const char * s, int Align);
TEXT_Handle TEXT_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
TEXT_Handle TEXT_CreateEx      (int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                                int WinFlags, int ExFlags, int Id, const char* pText);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/

/* Methods changing properties */

void TEXT_SetBkColor  (TEXT_Handle pObj, GUI_COLOR Color); /* Obsolete. Left in GUI for compatibility to older versions */
void TEXT_SetFont     (TEXT_Handle pObj, const GUI_FONT GUI_UNI_PTR * pFont);
void TEXT_SetText     (TEXT_Handle pObj, const char* s);
void TEXT_GetText			(TEXT_Handle hObj, char* sDest, int MaxLen);
void TEXT_SetTextAlign(TEXT_Handle pObj, int Align);
void TEXT_SetTextColor(TEXT_Handle pObj, GUI_COLOR Color);

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

void            TEXT_SetDefaultFont(const GUI_FONT GUI_UNI_PTR * pFont);
const GUI_FONT GUI_UNI_PTR * TEXT_GetDefaultFont(void);
void            TEXT_SetDefaultTextColor(GUI_COLOR Color);

/*********************************************************************
*
*       Internal: Object definition
*
**********************************************************************
*/

typedef struct {
  WIDGET Widget;
  WM_HMEM hpText;
  const GUI_FONT GUI_UNI_PTR * pFont;
  I16 Align;
  GUI_COLOR TextColor;
  GUI_COLOR BkColor;
  #if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
    int DebugId;
  #endif  
} TEXT_Obj;

#define TEXT_H2P(h) (TEXT_Obj*) GUI_ALLOC_h2p(h)

#if defined(__cplusplus)
  }
#endif

#endif   /* if GUI_WINSUPPORT */
#endif   /* TEXT_H */
