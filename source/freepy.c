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
#define NOIME 
#include "freepy.h"

#define CS_FREEPY (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS)


#pragma comment(lib,"IMM32.lib")

/**********************************************************************/
/*    DLLEntry                                                        */
/**********************************************************************/
BOOL WINAPI DllMain (
					 HINSTANCE    hInstDLL,
					 DWORD        dwFunction,
					 LPVOID       lpNot)
{
	
    switch(dwFunction)
    {
	case DLL_PROCESS_ATTACH:
		/* for debug ********************************/
#ifdef _MY_DEBUG
		if(nDebugLevel){
			if( (DebugLogFile=_tfopen( DEBUGLOGFILE, "w"))==NULL)
				MessageBox(NULL,"can not open Debuglogfile","debug",MB_OK);
			DebugLog(1,(DebugLogFile,"Entry in\n"));
		}
#endif
		/********************************************/

		InitDictionary();

		hInst = hInstDLL;
		IMERegisterClass( hInst );
		break;
		
	case DLL_PROCESS_DETACH:
		UnregisterClass(UICLASSNAME,hInst);
		UnregisterClass(COMPCLASSNAME,hInst);
		UnregisterClass(CANDCLASSNAME,hInst);
		UnregisterClass(STATUSCLASSNAME,hInst);

		DestroyDictionary();
		
		/* for debug ********************************/
#ifdef _MY_DEBUG
		if(nDebugLevel){
			DebugLog(1,(DebugLogFile,"Entry out\n"));
			if(DebugLogFile!=NULL)
				fclose(DebugLogFile);
		}
#endif
		/********************************************/
		break;
		
	case DLL_THREAD_ATTACH:
		break;
		
	case DLL_THREAD_DETACH:
		break;
    }
    return TRUE;
}

BOOL IMERegisterClass( HANDLE hInstance )
{
    WNDCLASSEX wc;
	
    //
    // register class of UI window.
    //
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_FREEPY | CS_IME;
    wc.lpfnWndProc    = UIWndProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = 2 * sizeof(LONG);
    wc.hInstance      = hInstance;
    wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon          = NULL;
    wc.lpszMenuName   = (LPTSTR)NULL;
    wc.lpszClassName  = UICLASSNAME;
    wc.hbrBackground  = NULL;
    wc.hIconSm        = NULL;
	
    if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
        return FALSE;
	
    //
    // register class of composition window.
    //
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_FREEPY | CS_IME;
    wc.lpfnWndProc    = CompWndProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = UICHILDEXTRASIZE;
    wc.hInstance      = hInstance;
    wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon          = NULL;
    wc.lpszMenuName   = (LPSTR)NULL;
    wc.lpszClassName  = COMPCLASSNAME;
    wc.hbrBackground  = NULL;
    wc.hIconSm        = NULL;
	
    if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
        return FALSE;
	
    //
    // register class of candadate window.
    //
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_FREEPY | CS_IME;
    wc.lpfnWndProc    = CandWndProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = UICHILDEXTRASIZE;
    wc.hInstance      = hInstance;
    wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon          = NULL;
    wc.lpszMenuName   = (LPSTR)NULL;
    wc.lpszClassName  = CANDCLASSNAME;
    wc.hbrBackground  = NULL;
    wc.hIconSm        = NULL;
	
    if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
        return FALSE;
	
    //
    // register class of status window.
    //
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_FREEPY | CS_IME;
    wc.lpfnWndProc    = StatusWndProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = UICHILDEXTRASIZE;
    wc.hInstance      = hInstance;
    wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon          = NULL;
    wc.lpszMenuName   = (LPSTR)NULL;
    wc.lpszClassName  = STATUSCLASSNAME;
    wc.hbrBackground  = NULL;
    wc.hIconSm        = NULL;
	
    if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
        return FALSE;
	
    return TRUE;
}


/**********************************************************************/
/*                                                                    */
/* UIWndProc()                                                   */
/*                                                                    */
/* IME UI window procedure                                            */
/*                                                                    */
/**********************************************************************/
LRESULT WINAPI UIWndProc(
				HWND hWnd,
				UINT message,
				WPARAM wParam,
				LPARAM lParam)
{
	
    HIMC           hUICurIMC;
    LPINPUTCONTEXT lpIMC;
    LPUIEXTRA      lpUIExtra;
    HGLOBAL        hUIExtra;
    LONG           lRet = 0L;
	
	DebugLog(1,(DebugLogFile,"UIWnd\n"));
	
    hUICurIMC = (HIMC)GetWindowLong(hWnd,IMMGWL_IMC);
	
    //
    // Even if there is no current UI. these messages should not be pass to 
    // DefWindowProc().
    //
    if (!hUICurIMC)
    {
        switch (message)
        {
		case WM_IME_STARTCOMPOSITION:
		case WM_IME_ENDCOMPOSITION:
		case WM_IME_COMPOSITION:
		case WM_IME_NOTIFY:
		case WM_IME_CONTROL:
		case WM_IME_COMPOSITIONFULL:
		case WM_IME_SELECT:
		case WM_IME_CHAR:
			return 0L;
		default:
			break;
        }
    }
	
    switch (message)
    {
	case WM_CREATE:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_CREATE\n"));
		
		//
		// Allocate UI's extra memory block.
		//
		hUIExtra = GlobalAlloc(GHND,sizeof(UIEXTRA));
		lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);

		lpUIExtra->uiComp.pt.x = -1;
		lpUIExtra->uiComp.pt.y = -1;

		CreateCompWindow(hWnd,lpUIExtra);
		CreateCandWindow(hWnd,lpUIExtra);

		GlobalUnlock(hUIExtra);
		SetWindowLong(hWnd,IMMGWL_PRIVATE,(DWORD)hUIExtra);
		break;
		
	case WM_IME_SETCONTEXT:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_SETCONTEXT\n"));
		if (wParam)
		{
			
			hUIExtra = (HGLOBAL)GetWindowLong(hWnd,IMMGWL_PRIVATE);
			lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);
			
			if (hUICurIMC)
			{
				//
				// input context was chenged.
				// if there are the child windows, the diplay have to be
				// updated.
				//
				lpIMC = ImmLockIMC(hUICurIMC);
				if (lpIMC)
				{
					MoveCandWindow(hWnd,lpUIExtra,lpIMC);
					MoveCompWindow(hWnd,lpUIExtra,lpIMC);
				}
				else
				{
					HideCandWindow(lpUIExtra);
					HideCompWindow(lpUIExtra);
				}
				ImmUnlockIMC(hUICurIMC);
			}
			else   // it is NULL input context.
			{
				HideCandWindow(lpUIExtra);
				HideCompWindow(lpUIExtra);
				
			}
			GlobalUnlock(hUIExtra);
		}
		break;
		
	case WM_IME_STARTCOMPOSITION:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_STARTCOMPOSITION\n"));
		//
		// Start composition! Ready to display the composition string.
		//
		break;
		
	case WM_IME_COMPOSITION:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_COMPOSITION\n"));
		
		//
		// Update to display the composition string.
		//
		lpIMC = ImmLockIMC(hUICurIMC);
		hUIExtra = (HGLOBAL)GetWindowLong(hWnd,IMMGWL_PRIVATE);
		lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);
		MoveCompWindow(hWnd,lpUIExtra,lpIMC);
		MoveCandWindow(hWnd,lpUIExtra,lpIMC);
		GlobalUnlock(hUIExtra);
		ImmUnlockIMC(hUICurIMC);
		break;
		
	case WM_IME_ENDCOMPOSITION:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_ENDCOMPOSITION\n"));
		
		//
		// Finish to display the composition string.
		//
		hUIExtra = (HGLOBAL)GetWindowLong(hWnd,IMMGWL_PRIVATE);
		lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);
		HideCompWindow(lpUIExtra);
		HideCandWindow(lpUIExtra);
		GlobalUnlock(hUIExtra);
		break;
		
	case WM_IME_COMPOSITIONFULL:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_COMPOSITIONFULL\n"));
		break;
		
	case WM_IME_SELECT:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_SELECT\n"));
		break;
		
	case WM_IME_CONTROL:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_CONTROL\n"));
		lRet = ControlHandle(hUICurIMC, hWnd,message,wParam,lParam);
		break;
		
		
	case WM_IME_NOTIFY:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY\n"));
		lRet = NotifyHandle(hUICurIMC, hWnd,message,wParam,lParam);
		break;
		
	case WM_DESTROY:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_DESTROY\n"));
		
		hUIExtra = (HGLOBAL)GetWindowLong(hWnd,IMMGWL_PRIVATE);
		lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);
		
		if (IsWindow(lpUIExtra->uiStatus.hWnd))
			DestroyWindow(lpUIExtra->uiStatus.hWnd);
		
		if (IsWindow(lpUIExtra->uiCand.hWnd))
			DestroyWindow(lpUIExtra->uiCand.hWnd);
		
		if (IsWindow(lpUIExtra->uiComp.hWnd))
			DestroyWindow(lpUIExtra->uiComp.hWnd);
		
		GlobalUnlock(hUIExtra);
		GlobalFree(hUIExtra);
		break;

	case WM_UI_COMPMOVE:
		hUIExtra = (HGLOBAL)GetWindowLong(hWnd,IMMGWL_PRIVATE);
		lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);
		lpUIExtra->uiComp.pt.x = (long)LOWORD(lParam);
		lpUIExtra->uiComp.pt.y = (long)HIWORD(lParam);
		GlobalUnlock(hUIExtra);
		break;

	default:
		return DefWindowProc(hWnd,message,wParam,lParam);
    }
    return lRet;
}

/**********************************************************************/
/*                                                                    */
/* NotifyHandle()                                                     */
/*                                                                    */
/* Handle WM_IME_NOTIFY messages.                                     */
/*                                                                    */
/**********************************************************************/

LONG NotifyHandle(HIMC hUICurIMC, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LONG lRet = 0L;
    LPINPUTCONTEXT lpIMC;
    HGLOBAL hUIExtra;
    LPUIEXTRA lpUIExtra;

    if (!(lpIMC = ImmLockIMC(hUICurIMC)))
        return 0L;

    hUIExtra = (HGLOBAL)GetWindowLong(hWnd,IMMGWL_PRIVATE);
    lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);

    switch (wParam)
    {
	case IMN_CLOSESTATUSWINDOW:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_CLOSESTATUSWINDOW\n"));
		if (IsWindow(lpUIExtra->uiStatus.hWnd)) {
			ShowWindow(lpUIExtra->uiStatus.hWnd,SW_HIDE);
		}
		break;
		
	case IMN_OPENSTATUSWINDOW:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_OPENSTATUSWINDOW\n"));
		CreateStatusWindow( hWnd, lpUIExtra);
		break;
		
	case IMN_OPENCANDIDATE:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_OPENCANDIDATE\n"));
		break;
		
	case IMN_CHANGECANDIDATE:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_CHANGECANDIDATE\n"));
		break;
		
	case IMN_CLOSECANDIDATE:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_CLOSECANDIDATE\n"));
		break;
		
	case IMN_SETCONVERSIONMODE:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_SETCONVERSIONMODE\n"));
		UpdateStatusWindow(lpUIExtra);
		break;
		
	case IMN_SETSENTENCEMODE:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_SETSENTENCEMODE\n"));
		break;
		
	case IMN_SETOPENSTATUS:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_SETOPENSTATUS\n"));
		UpdateStatusWindow(lpUIExtra);
		break;
		
	case IMN_SETCANDIDATEPOS:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_SETCANDIDATEPOS\n"));
		break;
		
	case IMN_SETCOMPOSITIONFONT:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_SETCOMPOSITIONFONT\n"));
		break;
		
	case IMN_SETCOMPOSITIONWINDOW:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_SETCOMPOSITIONWINDOW\n"));
		if(wConversionSet & CONVERSION_SET_FOLLOW) {
			POINT ptSrc;
			SIZE szOffset;
			HDC hDC;

			ptSrc = lpIMC->cfCompForm.ptCurrentPos;
			ClientToScreen(lpIMC->hWnd, &ptSrc);
			hDC = GetDC(lpIMC->hWnd);
			GetTextExtentPoint(hDC,"A",1,&szOffset);
			ReleaseDC(lpIMC->hWnd,hDC);

			lpUIExtra->uiComp.pt.x = ptSrc.x + szOffset.cx;
			lpUIExtra->uiComp.pt.y = ptSrc.y + szOffset.cy;
		}
	    if (IsWindow(lpUIExtra->uiComp.hWnd))
			InvalidateRect(lpUIExtra->uiComp.hWnd,NULL,FALSE);

		break;
		
	case IMN_GUIDELINE:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_GUIDELINE\n"));
		break;
		
	case IMN_SETSTATUSWINDOWPOS:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_SETSTATUSWINDOWPOS\n"));
		break;
		
	case IMN_PRIVATE:
		DebugLog(1,(DebugLogFile,"UIWnd:WM_IME_NOTIFY:IMN_PRIVATE\n"));
		break;
		
	default:
		break;
    }
    GlobalUnlock(hUIExtra);
    ImmUnlockIMC(hUICurIMC);

    return lRet;
}

/**********************************************************************/
/*                                                                    */
/* ControlHandle()                                                    */
/*                                                                    */
/* Handle WM_IME_CONTROL messages.                                    */
/*                                                                    */
/**********************************************************************/
LONG ControlHandle(HIMC hUICurIMC, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0L;
}

