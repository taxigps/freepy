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

LRESULT WINAPI CandWndProc( 
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam)
{
    switch (message)
    {
        case WM_PAINT:
            PaintCandWindow(hWnd);
            break;

        case WM_SETCURSOR:
        case WM_MOUSEMOVE:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
            DragUI(hWnd,NULL,message,wParam,lParam,FALSE);
            if ((message == WM_SETCURSOR) &&
                (HIWORD(lParam) != WM_LBUTTONDOWN) &&
                (HIWORD(lParam) != WM_RBUTTONDOWN)) 
                    return DefWindowProc(hWnd,message,wParam,lParam);
            if ((message == WM_LBUTTONUP) || (message == WM_RBUTTONUP))
                SetWindowLong(hWnd,FIGWL_MOUSE,0L);
            break;

        default:
            if (!MyIsIMEMessage(message)){
                return DefWindowProc(hWnd,message,wParam,lParam);
			}
            break;
    }
    return 0L;
}

void CreateCandWindow( HWND hUIWnd,LPUIEXTRA lpUIExtra)
{
    if (!IsWindow(lpUIExtra->uiCand.hWnd))
    {
		HDC hDC;
		TCHAR szStr[100];

        lpUIExtra->uiCand.hWnd = 
                CreateWindowEx(WS_EX_WINDOWEDGE,
                             CANDCLASSNAME,NULL,
                             WS_DISABLED | WS_POPUP | WS_DLGFRAME,
                             0,
                             0,
                             1,
							 1,
                             hUIWnd,NULL,hInst,NULL);
		SetWindowLong(lpUIExtra->uiCand.hWnd,FIGWL_SVRWND,(DWORD)hUIWnd);

		hDC = GetDC(lpUIExtra->uiCand.hWnd);
		
		_stprintf(szStr,_T("AAAAAAAAAAAAA"));
		GetTextExtentPoint(hDC,szStr,_tcslen(szStr),sizeCand);
		_stprintf(szStr,_T("<< 1가 2가 3가 4가 5가 6가 7가 8가 9가 0가 >>"));
		GetTextExtentPoint(hDC,szStr,_tcslen(szStr),sizeCand+1);
		ReleaseDC(lpUIExtra->uiCand.hWnd,hDC);
		sizeCand[0].cy *= 11;
		sizeCand[1].cy *= 2;
		sizeCand[1].cy -= 4;
		
//		if(wConversionSet & CONVERSION_SET_SHAPE) {
//			lpUIExtra->uiCand.sz.cx = sizeCand[1].cx;
//			lpUIExtra->uiCand.sz.cy = sizeCand[1].cy;
//		}
//		else {
			lpUIExtra->uiCand.sz.cx = sizeCand[0].cx;
			lpUIExtra->uiCand.sz.cy = sizeCand[0].cy;
//		}
    }
    ShowWindow(lpUIExtra->uiCand.hWnd, SW_HIDE);
    return;
}

BOOL GetCandPosFromCompWnd(LPUIEXTRA lpUIExtra,LPSIZE lpsz)
{
	if (IsWindow(lpUIExtra->uiComp.hWnd))
    {
		RECT rc,screenrc;
		POINT pt;

		GetWindowRect(lpUIExtra->uiComp.hWnd,&rc);

		if(wConversionSet & CONVERSION_SET_SHAPE) {	
			pt.x = rc.left;
			pt.y = rc.bottom;
		}
		else {
			pt.x = rc.right + 5;
			pt.y = rc.top;
		}
		SystemParametersInfo(SPI_GETWORKAREA,
							0,
							&screenrc,
							0);
		if(wConversionSet & CONVERSION_SET_SHAPE) {	
			if( (pt.x + sizeCand[1].cx) > screenrc.right)
				pt.x = screenrc.right - sizeCand[1].cx;
			if( (pt.y + sizeCand[1].cy) > screenrc.bottom)
				pt.y = rc.top - sizeCand[1].cy;
		}
		else {
			if( (pt.x + lpsz->cx) > screenrc.right)
				pt.x = rc.left - lpsz->cx - 5;
			if( (pt.y + lpsz->cy) > screenrc.bottom)
				pt.y = screenrc.bottom - lpsz->cy;
		}

		lpUIExtra->uiCand.pt.x = pt.x;
		lpUIExtra->uiCand.pt.y = pt.y;
		return TRUE;
    }
    return FALSE;
}


void MoveCandWindow(HWND hUIWnd,LPUIEXTRA lpUIExtra, LPINPUTCONTEXT lpIMC)
{
	if (!IsWindow(lpUIExtra->uiCand.hWnd))
		CreateCandWindow( hUIWnd, lpUIExtra);

	if (IsWindow(lpUIExtra->uiCand.hWnd))
	{
	    LPCANDIDATEINFO lpCandInfo;
	    LPCANDIDATELIST lpCandList;
		HDC hDC;
		SIZE sz;
		LPTSTR lpStr;

		sz.cx = 0;
		sz.cy = 0;

		if (lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo))
		{
			lpCandList = (LPCANDIDATELIST)((LPSTR)lpCandInfo  + lpCandInfo->dwOffset[0]);
			if( lpCandList->dwCount <= 0) {
				ShowWindow(lpUIExtra->uiCand.hWnd, SW_HIDE);
				ImmUnlockIMCC(lpIMC->hCandInfo);
				return;
			}
			lpStr = GETLPCANDSTR(lpCandList,1);
			hDC = GetDC(lpUIExtra->uiCand.hWnd);
			GetTextExtentPoint(hDC,lpStr,_tcslen(lpStr),&sz);
			ReleaseDC(lpUIExtra->uiCand.hWnd,hDC);
			if(_tcslen(lpStr))	sz.cx += 3*sz.cx/_tcslen(lpStr);
			ImmUnlockIMCC(lpIMC->hCandInfo);
		}
		if(wConversionSet & CONVERSION_SET_SHAPE) {	
			sz.cx = sizeCand[1].cx;
			sz.cy = sizeCand[1].cy;
		}
		else{
			if(sz.cx < lpUIExtra->uiCand.sz.cx)
					sz.cx = lpUIExtra->uiCand.sz.cx;
			sz.cy = lpUIExtra->uiCand.sz.cy;
		}

		GetCandPosFromCompWnd(lpUIExtra,&sz);

		MoveWindow(lpUIExtra->uiCand.hWnd,
			lpUIExtra->uiCand.pt.x,
			lpUIExtra->uiCand.pt.y,
			sz.cx,
			sz.cy,
			TRUE);
		ShowWindow(lpUIExtra->uiCand.hWnd,SW_SHOWNOACTIVATE);
		InvalidateRect(lpUIExtra->uiCand.hWnd,NULL,FALSE);
	}
}

void PaintCandWindow( HWND hCandWnd)
{
    PAINTSTRUCT ps;
    HIMC hIMC;
    LPINPUTCONTEXT lpIMC;
    LPCANDIDATEINFO lpCandInfo;
    LPCANDIDATELIST lpCandList;
    HDC hDC;
    RECT rc;
    LPTSTR lpstr;
	TCHAR szStr[100];
    int height,width;
    DWORD i;
    SIZE sz;
    HWND hSvrWnd;
	HBRUSH hBrush = (HBRUSH)NULL;
	HBRUSH hOldBrush = (HBRUSH)NULL;
	HPEN hPen = (HPEN)NULL;
	HPEN hOldPen = (HPEN)NULL;
	
    hDC = BeginPaint(hCandWnd,&ps);
	
	GetClientRect(hCandWnd,&rc);
	hBrush = GetStockObject(LTGRAY_BRUSH);
	hOldBrush=SelectObject(hDC,hBrush);
	PatBlt(hDC,
			rc.left,
			rc.top ,
			rc.right,
			rc.bottom,
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

    hSvrWnd = (HWND)GetWindowLong(hCandWnd,FIGWL_SVRWND);
	
    if (hIMC = (HIMC)GetWindowLong(hSvrWnd,IMMGWL_IMC))
    {
        lpIMC = ImmLockIMC(hIMC);
        if (lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo))
        {
			DWORD dwMaxPaintPos;
			WORD  wCount;

            height = 3;
			width = 1;
            lpCandList = (LPCANDIDATELIST)((LPSTR)lpCandInfo  + lpCandInfo->dwOffset[0]);
			SetBkMode(hDC,TRANSPARENT);
			SetPaintColor(hDC, wConversionMode);

			if(wConversionSet & CONVERSION_SET_SHAPE) {
				lpstr = GETLPCANDSTR(lpCandList,0);
				TextOut(hDC,width,height,lpstr,_tcslen(lpstr));
				GetTextExtentPoint(hDC,lpstr,_tcslen(lpstr),&sz);
				width += sz.cx*3/2;
			}

			dwMaxPaintPos = lpCandList->dwPageStart +lpCandList->dwPageSize ;
			if(dwMaxPaintPos > lpCandList->dwCount + 2)  dwMaxPaintPos = lpCandList->dwCount + 2;

			wCount = 0;
            for (i = lpCandList->dwPageStart; i < dwMaxPaintPos; i++)
            {
				wCount++;
                lpstr = GETLPCANDSTR(lpCandList,i);
				GetTextExtentPoint(hDC,lpstr,_tcslen(lpstr),&sz);
				if(wConversionSet & CONVERSION_SET_SHAPE) {
					_stprintf(szStr,"%d%s ",wCount%10,lpstr);
					TextOut(hDC,width,height,szStr,_tcslen(szStr));
					GetTextExtentPoint(hDC,szStr,_tcslen(szStr),&sz);
					width += sz.cx;
				}
				else {
					_stprintf(szStr,"%d %s ",wCount%10,lpstr);
					TextOut(hDC,width,height,szStr,_tcslen(szStr));
					GetTextExtentPoint(hDC,szStr,_tcslen(szStr),&sz);
					height += sz.cy;
				}
            }
			if(wConversionSet & CONVERSION_SET_SHAPE) {
				lpstr = GETLPCANDSTR(lpCandList,1);
				if(_tcslen(lpstr)) {
					GetTextExtentPoint(hDC,"A",1,&sz);
					width = sizeCand[1].cx - sz.cx*3/2;
					TextOut(hDC,width,height,lpstr,_tcslen(lpstr));
				}
			}

            ImmUnlockIMCC(lpIMC->hCandInfo);
        }
        ImmUnlockIMC(hIMC);
    }
    EndPaint(hCandWnd,&ps);
}

void HideCandWindow( LPUIEXTRA lpUIExtra)
{
    if (IsWindow(lpUIExtra->uiCand.hWnd))
    {
        ShowWindow(lpUIExtra->uiCand.hWnd, SW_HIDE);
    }
}
