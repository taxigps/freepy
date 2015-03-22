/*
 * Copyright (C) 1999.4  Li ZhenChun
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License; or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that is will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, M A 02139, USA.
 *
 * Author: Li ZhenChun  email: zhchli@163.net or zhchli@126.com
 * 
 */

#ifndef _FREEPY_H_
#define _FREEPY_H_
#define NOIME 
#include <tchar.h>

#include <windows.h>
#include <stdio.h>
#include "Imm.h"
#include "indicml.h"
#include "vksub.h"
#include "HZfreepy.h"
#include "resource.h"

/**********************************************************************/
/*                                                                    */
/*      Define                                                        */
/*                                                                    */
/**********************************************************************/
#define GB_QUFIRST  0xA1
#define GB_QULAST   0xF7
#define GB_WEIFIRST 0xA1
#define GB_WEILAST  0xFE

#define PHTOCHMODEL      _T("词定字模式：")
#define IMODEL           _T("i模式：")
#define UMODEL           _T("u模式：")
#define VMODEL           _T("v模式：")
#define ATOMTIP          _T("自由拼音")

#define FREEPYFILENAME   _T("freepy.ime")
#define UICLASSNAME      _T("FREEPYUI")
#define COMPCLASSNAME    _T("FREEPYCOMP")
#define CANDCLASSNAME    _T("FREEPYCAND")
#define STATUSCLASSNAME  _T("FREEPYSTATUS")

#define EDIT_ADD         1
#define EDIT_BACK        2
#define EDIT_DELETE      3

/* Conversion Mode */

#define CONVERSION_SET_PUNCT        0x01
#define CONVERSION_SET_FULLSHAPE    0x02
#define CONVERSION_SET_SORT         0x0100
#define CONVERSION_SET_FUZZYC       0x0200
#define CONVERSION_SET_FUZZYN       0x0400
#define CONVERSION_SET_FUZZYH       0x0800
#define CONVERSION_SET_FUZZYB       0x1000
#define CONVERSION_SET_FOLLOW       0x2000
#define CONVERSION_SET_GBK          0x4000
#define CONVERSION_SET_SHAPE        0x8000

#define CONVERSION_MODE_PHRASETOCHAR 0x01
#define CONVERSION_MODE_I            0x02
#define CONVERSION_MODE_U            0x04
#define CONVERSION_MODE_V            0x08


/* The flags of FIGWL_MOUSE */
#define FIM_CAPUTURED 0x01
#define FIM_MOVED     0x02

/* special messages */
#define WM_UI_COMPMOVE (WM_USER+601)

/* WndExtra of child UI windows */
#define FIGWL_MOUSE         4
#define FIGWL_SVRWND        8
#define UICHILDEXTRASIZE    12

/* define GET LP for COMPOSITIONSTRING members. */
#define GETLPCOMPSTR(lpcs) (LPTSTR)((LPBYTE)(lpcs) + (lpcs)->dwCompStrOffset)
#define GETLPRESULTSTR(lpcs) (LPTSTR)((LPBYTE)(lpcs) + (lpcs)->dwResultStrOffset)

#define GETLPCANDSTR(lpcs,i) (LPTSTR)((LPBYTE)(lpcs) + (lpcs)->dwOffset[i])

/**********************************************************************/
/*                                                                    */
/*      Externs                                                       */
/*                                                                    */
/**********************************************************************/
#ifndef _NO_EXTERN_

extern HINSTANCE  hInst;
extern HKL        hFreePYKL;
extern LPDWORD    lpdwCurTransKey;
extern UINT       uNumTransKey;
extern BOOL       fOverTransKey;
extern WORD       wConversionMode;
extern WORD       wConversionSet;
extern SIZE       sizeCand[2];

extern BYTE bComp[];
extern BYTE bNoComp[];


#endif //_NO_EXTERN_

/**********************************************************************/
/*                                                                    */
/*      Structures                                                    */
/*                                                                    */
/**********************************************************************/
typedef struct _tagMYCOMPSTR{
    COMPOSITIONSTRING cs;
    TCHAR             szCompStr[MAXCOMPSIZE];
    TCHAR             szResultStr[MAXCOMPSIZE];

	FREEPYCOMP        FreePYComp;

} MYCOMPSTR, NEAR *PMYCOMPSTR, FAR *LPMYCOMPSTR;

typedef struct _tagMYCAND{
    CANDIDATEINFO  ci;
    CANDIDATELIST  cl;
    DWORD          offset[MAXCANDSTRNUM];
    TCHAR          szCandStr[MAXCANDSTRNUM][MAXCANDSTRSIZE];

	FREEPYCAND     FreePYCand;

} MYCAND, NEAR *PMYCAND, FAR *LPMYCAND;

typedef struct _tagUICHILD{
    HWND    hWnd;
	POINT   pt;
	SIZE    sz;
} UICHILD, NEAR *PUICHILD, FAR *LPUICHILD;

typedef struct _tagUIEXTRA{
    UICHILD  uiStatus;
    UICHILD  uiCand;
	UICHILD  uiComp;
} UIEXTRA, NEAR *PUIEXTRA, FAR *LPUIEXTRA;

typedef struct _tagGENEMSG{
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;
} GENEMSG, NEAR *PGENEMSG, FAR *LPGENEMSG;


/**********************************************************************/
/*                                                                    */
/*      Functions                                                     */
/*                                                                    */
/**********************************************************************/
//freepy.c
BOOL WINAPI DllMain (HINSTANCE,DWORD,LPVOID);
BOOL IMERegisterClass( HANDLE);
LRESULT WINAPI UIWndProc(HWND,UINT,WPARAM,LPARAM);
LONG NotifyHandle(HIMC, HWND, UINT, WPARAM, LPARAM);
LONG ControlHandle(HIMC , HWND , UINT , WPARAM , LPARAM );

//handle.c
BOOL IMEKeyupHandler( HIMC,WPARAM,LPARAM,LPBYTE);
BOOL IMEKeydownHandler( HIMC,WPARAM,LPARAM,LPBYTE);
BOOL KeydownHandler( HIMC,WORD,LONG,LPBYTE);
BOOL CharHandler( HIMC,WORD,LONG);

//subs.c
BOOL IsCompStr(HIMC);
void InitCompStr(LPCOMPOSITIONSTRING);
void ClearCompStr(LPCOMPOSITIONSTRING);
BOOL IsCandidate(HIMC);
void InitCandInfo(LPCANDIDATEINFO);
void ClearCandidate(LPCANDIDATEINFO);
HKL GetMyHKL();
void UpdateIndicIcon(HIMC);
BOOL MyIsIMEMessage(UINT);
BOOL MakeResultString( HIMC,BOOL);
BOOL GenerateMessage(HIMC, LPDWORD,LPGENEMSG);
BOOL GenerateMessageToTransKey(LPDWORD,LPGENEMSG);
void DrawUIBorder( LPRECT );
void DragUI( HWND ,HWND, UINT , WPARAM , LPARAM,BOOL);
void SelectCandFromCandlist( HIMC, WORD );
void DeleteCharBackward(HIMC,WORD);
void DeleteCharForward(HIMC,WORD);
void SpaceSelect(HIMC,WORD);
void SetPaintColor(HDC , WORD);
void SelectForwardFromCand(HIMC,LPCANDIDATELIST);
void SelectBackwardFromCand(HIMC,LPCANDIDATELIST);
BOOL WINAPI ConfigDialogProc(HWND ,UINT,WPARAM,LPARAM);


//uicand.c
LRESULT WINAPI CandWndProc(HWND,UINT,WPARAM,LPARAM);
void CreateCandWindow( HWND ,LPUIEXTRA );
void MoveCandWindow(HWND , LPUIEXTRA , LPINPUTCONTEXT);
void PaintCandWindow( HWND);
void HideCandWindow( LPUIEXTRA );

//uicomp.c
LRESULT WINAPI CompWndProc(HWND,UINT,WPARAM,LPARAM);
void CreateCompWindow( HWND, LPUIEXTRA);
void MoveCompWindow( HWND,LPUIEXTRA ,LPINPUTCONTEXT);
void PaintCompWindow( HWND );
void HideCompWindow(LPUIEXTRA);

//uistatus.c
LRESULT WINAPI StatusWndProc(HWND,UINT,WPARAM,LPARAM);
void UpdateStatusWindow(LPUIEXTRA );
void CreateStatusWindow( HWND , LPUIEXTRA );

#endif /* _FREEPY_H_ */