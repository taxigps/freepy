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

#include "freepy.h"

LRESULT WINAPI CompWndProc(
			HWND   hWnd,
			UINT   message,
			WPARAM wParam,
			LPARAM lParam)
{
	HWND  hUIWnd;
    HGLOBAL hUIExtra;
    LPUIEXTRA lpUIExtra;

	hUIWnd = (HWND)GetWindowLong(hWnd,FIGWL_SVRWND);
    hUIExtra = (HGLOBAL)GetWindowLong(hUIWnd,IMMGWL_PRIVATE);
	lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);

    switch (message)
    {
        case WM_PAINT:
            PaintCompWindow( hWnd);
            break;

        case WM_SETCURSOR:
        case WM_MOUSEMOVE:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
            DragUI(hWnd,lpUIExtra->uiCand.hWnd,message,wParam,lParam,TRUE);
            if ((message == WM_SETCURSOR) &&
                (HIWORD(lParam) != WM_LBUTTONDOWN) &&
                (HIWORD(lParam) != WM_RBUTTONDOWN)) 
                return DefWindowProc(hWnd,message,wParam,lParam);
            if ((message == WM_LBUTTONUP) || (message == WM_RBUTTONUP))
                SetWindowLong(hWnd,FIGWL_MOUSE,0L);
            break;

        default:
            if (!MyIsIMEMessage(message)) {
				GlobalUnlock(hUIExtra);
                return DefWindowProc(hWnd,message,wParam,lParam);
			}
            break;
    }
    GlobalUnlock(hUIExtra);
    return 0;
}

void CreateCompWindow( HWND hUIWnd, LPUIEXTRA lpUIExtra)
{
    if (!IsWindow(lpUIExtra->uiComp.hWnd))
    {
		HDC hDC;
		SIZE sz;
		TCHAR szStr[100];

		lpUIExtra->uiComp.hWnd = 
            CreateWindowEx( WS_EX_WINDOWEDGE,
                         COMPCLASSNAME,NULL,
                         WS_DISABLED | WS_POPUP | WS_DLGFRAME,
                         0,
                         0,
                         1,
						 1,
                         hUIWnd,NULL,hInst,NULL);
		SetWindowLong(lpUIExtra->uiComp.hWnd,FIGWL_SVRWND,(DWORD)hUIWnd);
	
		_stprintf(szStr,_T("AAAAAAAAAAAAA"));
		hDC = GetDC(lpUIExtra->uiComp.hWnd);
		GetTextExtentPoint(hDC,szStr,_tcslen(szStr),&sz);
		ReleaseDC(lpUIExtra->uiComp.hWnd,hDC);

		lpUIExtra->uiComp.sz.cx = sz.cx;
		lpUIExtra->uiComp.sz.cy = sz.cy+4;
    }
    ShowWindow(lpUIExtra->uiComp.hWnd,SW_HIDE);
    return;
}

void MoveCompWindow( HWND hUIWnd, LPUIEXTRA lpUIExtra, LPINPUTCONTEXT lpIMC)
{
	if (!IsWindow(lpUIExtra->uiComp.hWnd))
		CreateCompWindow( hUIWnd, lpUIExtra);

	if (IsWindow(lpUIExtra->uiComp.hWnd))
	{
		HDC hDC;
		LPCOMPOSITIONSTRING lpCompStr;
		POINT pt;
		RECT screenrc;
		LPTSTR lpStr;
		SIZE sz;

		sz.cx = 0;
		sz.cy = 0;

		if (lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr))
		{
			if ((lpCompStr->dwSize > sizeof(COMPOSITIONSTRING))
				&& (lpCompStr->dwCompStrLen > 0))
			{
				lpStr = 
					((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPaintCompStr;

				hDC = GetDC(lpUIExtra->uiComp.hWnd);
				GetTextExtentPoint(hDC,lpStr,_tcslen(lpStr),&sz);
				ReleaseDC(lpUIExtra->uiComp.hWnd,hDC);
				if(_tcslen(lpStr))	sz.cx += 2*sz.cx/_tcslen(lpStr);
				ImmUnlockIMCC(lpIMC->hCompStr);
			}
			else{
				ShowWindow(lpUIExtra->uiComp.hWnd, SW_HIDE);
				ImmUnlockIMCC(lpIMC->hCompStr);
				return;
			}

		}

		if(sz.cx < lpUIExtra->uiComp.sz.cx)
			sz.cx = lpUIExtra->uiComp.sz.cx;

		sz.cy = lpUIExtra->uiComp.sz.cy;
		sz.cx += 4 * GetSystemMetrics(SM_CXEDGE);
		sz.cy += 4 * GetSystemMetrics(SM_CYEDGE);

		if(lpUIExtra->uiComp.pt.x < 0) {
			pt.x = 15;
			pt.y = 15;
			ClientToScreen(lpIMC->hWnd, &pt);
		}
		else{
			pt.x = lpUIExtra->uiComp.pt.x;
			pt.y = lpUIExtra->uiComp.pt.y;
		}

		SystemParametersInfo(SPI_GETWORKAREA,
							0,
							&screenrc,
							0);

		if( (pt.x + sz.cx) > screenrc.right )
			pt.x = screenrc.right - sz.cx;
		if( (pt.y + sz.cy) > screenrc.bottom )
			pt.y = screenrc.bottom - sz.cy;

		MoveWindow(lpUIExtra->uiComp.hWnd,
			pt.x,
			pt.y,
			sz.cx,
			sz.cy,
			TRUE);

		ShowWindow(lpUIExtra->uiComp.hWnd, SW_SHOWNOACTIVATE);
		InvalidateRect(lpUIExtra->uiComp.hWnd,NULL,FALSE);
	}
}

void PaintCompWindow( HWND hCompWnd)
{
    PAINTSTRUCT ps;
    HIMC hIMC;
    LPINPUTCONTEXT lpIMC;
    LPCOMPOSITIONSTRING lpCompStr;
    HDC hDC;
    RECT rc;
    HWND hSvrWnd;
	HBRUSH hBrush = (HBRUSH)NULL;
	HBRUSH hOldBrush = (HBRUSH)NULL;
	HPEN hPen = (HPEN)NULL;
	HPEN hOldPen = (HPEN)NULL;

    hDC = BeginPaint(hCompWnd,&ps);

	GetClientRect(hCompWnd,&rc);

	hBrush = GetStockObject(LTGRAY_BRUSH);
	hOldBrush=SelectObject(hDC,hBrush);
	PatBlt(hDC,
			rc.left,
			rc.top ,
			rc.right - GetSystemMetrics(SM_CXEDGE)/2,
			rc.bottom - GetSystemMetrics(SM_CYEDGE)/2,
			PATCOPY);
	if(hBrush && hOldBrush)
		SelectObject(hDC,hOldBrush);

	hPen = GetStockObject(WHITE_PEN);
	hOldPen = SelectObject(hDC,hPen);
	MoveToEx(hDC,0,rc.bottom - GetSystemMetrics(SM_CXEDGE)/2,NULL);
	LineTo(hDC,rc.right-GetSystemMetrics(SM_CXEDGE)/2,rc.bottom - GetSystemMetrics(SM_CXEDGE)/2);
	LineTo(hDC,rc.right-GetSystemMetrics(SM_CXEDGE)/2,0);

	hPen = CreatePen(PS_SOLID ,0,RGB(128,128,128));
	SelectObject(hDC,hPen);
	MoveToEx(hDC,rc.right-GetSystemMetrics(SM_CXEDGE)/2,0,NULL);
	LineTo(hDC,0,0);
	LineTo(hDC,0,rc.bottom-GetSystemMetrics(SM_CYEDGE)/2);

	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);

    hSvrWnd = (HWND)GetWindowLong(hCompWnd,FIGWL_SVRWND);

    if (hIMC = (HIMC)GetWindowLong(hSvrWnd,IMMGWL_IMC))
    {
		if( (lpIMC = ImmLockIMC(hIMC)) == NULL ){
			EndPaint(hCompWnd,&ps);
			return;
		}
        if (lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr))
        {
            if ((lpCompStr->dwSize > sizeof(COMPOSITIONSTRING))
               && (lpCompStr->dwCompStrLen > 0))
            {
                LPTSTR lpStr,lpPaintStr;
				SIZE sz,sz1;
				SHORT wEditCaret;
				SHORT wCharNum,wCount;
				int i;

				wEditCaret = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.wEditCaret;
				lpStr = GETLPCOMPSTR(lpCompStr);
				lpPaintStr = 
					((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPaintCompStr;
				SetBkMode(hDC,TRANSPARENT);
				SetPaintColor(hDC, wConversionMode);
				TextOut(hDC,2,2,lpPaintStr,_tcslen(lpPaintStr));

				wCharNum = _tcslen(lpStr) - wEditCaret;
				wCount = 0;
				for(i = _tcslen(lpPaintStr);i;i--) {
					if(wCount == wCharNum) break;
					if( *(lpPaintStr + i -1 ) != _T(' ')) wCount++;
				}

				GetTextExtentPoint(hDC,lpPaintStr,i,&sz);
				GetTextExtentPoint(hDC,"A",1,&sz1);

				hPen = CreatePen(PS_SOLID,3,RGB(0,0,0));
				hOldPen = SelectObject(hDC,hPen);
				MoveToEx(hDC,sz.cx + sz1.cx/4,sz1.cy*11/10,NULL);
				LineTo(hDC,sz.cx + sz1.cx*2/3,sz1.cy*11/10);
				SelectObject(hDC,hOldPen);
				DeleteObject(hPen);
            }
            ImmUnlockIMCC(lpIMC->hCompStr);
        }
        ImmUnlockIMC(hIMC);
    }
    EndPaint(hCompWnd,&ps);
}

void HideCompWindow(LPUIEXTRA lpUIExtra)
{
    if (IsWindow(lpUIExtra->uiComp.hWnd))
    {
        ShowWindow(lpUIExtra->uiComp.hWnd, SW_HIDE);
    }
}