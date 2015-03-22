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

#define HELPFILENAME      _T("freepy.hlp")

#define TIMEID            1
#define TIMEOUT           600

#define BITMAPX           20
#define BITMAPY           20
#define BITMAPCHINESEDES  60
#define BITMAPFULLDES     80
#define BITMAPPUNCTDES    100
#define BITMAPCHINESESRC  60
#define BITMAPFULLSRC     100
#define BITMAPPUNCTSRC    140

#define BUTTONMENU        0x01
#define BUTTONPY          0x02
#define BUTTONCHINESE     0x04
#define BUTTONFULL        0x08
#define BUTTONPUNCT       0x10
#define BUTTONPUSH        0x8000

#define SETFUZZY          0x01
#define SETSORT           0x02

static HBITMAP hBmpStatus;

void PaintStatus( HWND,HDC,DWORD);
DWORD CheckButtonPos( HWND , LPPOINT);
BOOL WINAPI SetDialogProc(HWND,UINT,WPARAM,LPARAM);
BOOL WINAPI AboutDialogProc(HWND,UINT,WPARAM,LPARAM);

LRESULT WINAPI StatusWndProc(
			HWND hWnd ,
			UINT message ,
			WPARAM wParam ,
			LPARAM lParam )
{
	static BOOL  fCanMove = FALSE;
    static POINT ptdif;
    static RECT  drc;
    static SIZE  sz;
	static DWORD dwButton;

    DWORD        dwT;
    POINT        pt;
	HDC          hDC;
	HKEY         hKey,hChildKey;
	DWORD        dwDisposition;

	hDC = GetDC(hWnd);
    switch (message)
    {
        case WM_CREATE:
            hBmpStatus = LoadBitmap(hInst,MAKEINTRESOURCE(STATUSBITMAP));
            break;

        case WM_PAINT:
			{
			    PAINTSTRUCT ps;
			    HDC hPaintDC;

				hPaintDC = BeginPaint(hWnd,&ps);
		        PaintStatus(hWnd ,hPaintDC, 0);
				EndPaint(hWnd,&ps);
			    break;
			}

		case WM_TIMER:
			KillTimer(hWnd,TIMEID);
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));
			fCanMove = TRUE;
			dwButton &= ~BUTTONPUSH;
			PaintStatus(hWnd,hDC,dwButton);
			break;

        case WM_SETCURSOR:
			if ( HIWORD(lParam) == WM_LBUTTONDOWN ) {
				SetCapture(hWnd);

				GetCursorPos( &pt );
				GetWindowRect(hWnd,&drc);
				ptdif.x = pt.x - drc.left;
				ptdif.y = pt.y - drc.top;
				sz.cx = drc.right - drc.left;
				sz.cy = drc.bottom - drc.top;
				SetWindowLong(hWnd,FIGWL_MOUSE,FIM_CAPUTURED);

				dwButton = CheckButtonPos( hWnd, &pt);
				dwButton |= BUTTONPUSH;
				PaintStatus(hWnd,hDC,dwButton);

				if( dwButton & BUTTONPY)
					SetTimer(hWnd,TIMEID,1,NULL);
				else
					SetTimer(hWnd,TIMEID,TIMEOUT,NULL);
			}
			else 
				return DefWindowProc(hWnd,message,wParam,lParam);
			break;

        case WM_LBUTTONUP:
			KillTimer(hWnd,TIMEID);
			ReleaseCapture();
			if( fCanMove ) {
				dwT = GetWindowLong(hWnd,FIGWL_MOUSE);
				if (dwT & FIM_MOVED) {
					DrawUIBorder(&drc);
					GetCursorPos( &pt );
					MoveWindow(hWnd,pt.x - ptdif.x,
							pt.y - ptdif.y,
							sz.cx,
							sz.cy,TRUE);
				}
				SetCursor(LoadCursor(NULL,IDC_ARROW));
			}
			else{
				if( dwButton & BUTTONCHINESE ) {
				    HWND hSvrWnd;
					HIMC hIMC;
					LPINPUTCONTEXT lpIMC;

					hSvrWnd = (HWND)GetWindowLong(hWnd,FIGWL_SVRWND);
					hIMC = (HIMC)GetWindowLong(hSvrWnd,IMMGWL_IMC);
			        lpIMC = ImmLockIMC(hIMC);
					if(lpIMC->fOpen) {
						lpIMC->fOpen=FALSE;
						MakeResultString(hIMC,FALSE);
					}
					else lpIMC->fOpen=TRUE;
				}

				if( dwButton & BUTTONFULL){
					if(wConversionSet & CONVERSION_SET_FULLSHAPE)
						wConversionSet &= ~CONVERSION_SET_FULLSHAPE;
					else
						wConversionSet |= CONVERSION_SET_FULLSHAPE;
				}

				if( dwButton & BUTTONPUNCT ){
					if(wConversionSet & CONVERSION_SET_PUNCT)
						wConversionSet &= ~CONVERSION_SET_PUNCT;
					else
						wConversionSet |= CONVERSION_SET_PUNCT;
				}

				if( dwButton & BUTTONMENU ){
					HMENU hMenu,hSubMenu;
					POINT pt;
					int cmd;

					hMenu = LoadMenu(hInst, MAKEINTRESOURCE(FREEPYMENU));
					hSubMenu = GetSubMenu(hMenu,0);
 
					pt.x = 0;
					pt.y = 0;
					ClientToScreen(hWnd, &pt);

	                cmd = TrackPopupMenu(hSubMenu, 
                                     TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RETURNCMD, 
                                     pt.x,
                                     pt.y,
                                     0, 
                                     hWnd, 
                                     NULL);
					switch (cmd) {
					case IDM_SET:
						DialogBox( hInst,MAKEINTRESOURCE(DIALOGSET),NULL,SetDialogProc);
						InvalidateRect(hWnd,NULL,FALSE);
						break;
					case IDM_HELP:
						{
							TCHAR szHelpFileName[100];
							LPTSTR lpHelpFileName;

							lpHelpFileName = szHelpFileName;
							lpHelpFileName += GetSystemDirectory(szHelpFileName,100);
							if (*(lpHelpFileName-1) != _T('\\'))
								*lpHelpFileName++ = _T('\\');
							_tcscpy(lpHelpFileName,HELPFILENAME);
							WinHelp(NULL,szHelpFileName,HELP_CONTENTS,0);
							InvalidateRect(hWnd,NULL,FALSE);
						}
						break;
					case IDM_ABOUT:
						DialogBox( hInst,MAKEINTRESOURCE(DIALOGABOUT),NULL,AboutDialogProc);
						InvalidateRect(hWnd,NULL,FALSE);
						break;
					default:
						break;
					}

		            if (hMenu) DestroyMenu(hMenu);
				}

				dwButton &= ~BUTTONPUSH;
				PaintStatus(hWnd,hDC,dwButton);
			}

			SetWindowLong(hWnd,FIGWL_MOUSE,0);
			fCanMove = FALSE;
			break;
        
        case WM_MOUSEMOVE:
			if( fCanMove ) {
				dwT = GetWindowLong(hWnd,FIGWL_MOUSE);
				if (dwT & FIM_MOVED) {
					DrawUIBorder(&drc);
					GetCursorPos( &pt );
					drc.left   = pt.x - ptdif.x;
					drc.top    = pt.y - ptdif.y;
					drc.right  = drc.left + sz.cx;
					drc.bottom = drc.top + sz.cy;
					DrawUIBorder(&drc);
				}
				else if (dwT & FIM_CAPUTURED)
				{
					DrawUIBorder(&drc);
					SetWindowLong(hWnd,FIGWL_MOUSE,dwT | FIM_MOVED);
				}
			}
//			else KillTimer(hWnd,TIMEID);
			break;

        case WM_DESTROY:
            DeleteObject(hBmpStatus);
            break;

		case WM_MOVE:
			if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				_T("Software"),
				0,
				KEY_READ,
				&hKey) == ERROR_SUCCESS ){
				
				if( RegCreateKeyEx( hKey,
					_T("freepy"),
					0,
					NULL,
					REG_OPTION_NON_VOLATILE ,
					KEY_ALL_ACCESS ,
					NULL,
					&hChildKey,
					&dwDisposition ) == ERROR_SUCCESS) {
						RegSetValueEx(hChildKey,
							_T("Position"),
							0,
							REG_DWORD,
							(LPBYTE)&lParam,
							sizeof(DWORD));
					RegCloseKey(hChildKey);
				}
				
				RegCloseKey(hKey);
			}
			break;

        default:
            if (!MyIsIMEMessage(message))
                return DefWindowProc(hWnd,message,wParam,lParam);
            break;
    }

	ReleaseDC(hWnd,hDC);
    return 0;
}

void CreateStatusWindow( HWND hUIWnd, LPUIEXTRA lpUIExtra)
{
    if (!IsWindow(lpUIExtra->uiStatus.hWnd))
    {
		RECT screenrc;
		HKEY hKey;
		DWORD dwPosition,dwData=sizeof(DWORD);
		BOOL fFlag=TRUE;

		lpUIExtra->uiStatus.sz.cx = BITMAPX * 6 ;
		lpUIExtra->uiStatus.sz.cy = BITMAPY ;

		if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			_T("Software\\freepy"),
			0,
			KEY_READ,
			&hKey) == ERROR_SUCCESS ){
			if( RegQueryValueEx( hKey,
								_T("Position"),
								NULL,
								NULL,
								(LPBYTE)&dwPosition,
								&dwData) == ERROR_SUCCESS ){
				lpUIExtra->uiStatus.pt.x = (int)(short) LOWORD(dwPosition);
				lpUIExtra->uiStatus.pt.y = (int)(short) HIWORD(dwPosition);
				fFlag = FALSE;
			}
			RegCloseKey(hKey);
		}

		if(fFlag){
			SystemParametersInfo(SPI_GETWORKAREA,
							0,
							&screenrc,
							0);

			lpUIExtra->uiStatus.pt.x = screenrc.left + 10;
			lpUIExtra->uiStatus.pt.y = screenrc.bottom - lpUIExtra->uiStatus.sz.cy - 5;
		}

		lpUIExtra->uiStatus.hWnd = 
                      CreateWindowEx( 0,
                            STATUSCLASSNAME,NULL,
                            WS_DISABLED | WS_POPUP,
                            lpUIExtra->uiStatus.pt.x,
                            lpUIExtra->uiStatus.pt.y,
							lpUIExtra->uiStatus.sz.cx,
							lpUIExtra->uiStatus.sz.cy,
                            hUIWnd,NULL,hInst,NULL);
		SetWindowLong(lpUIExtra->uiStatus.hWnd,FIGWL_SVRWND,(DWORD)hUIWnd);
	}

	ShowWindow(lpUIExtra->uiStatus.hWnd,SW_SHOWNOACTIVATE);
    return;
}

DWORD CheckButtonPos( HWND hStatusWnd, LPPOINT lppt)
{
    POINT pt;
    RECT rc;
	DWORD dwButton = 0;

    if (lppt)
    {
        pt = *lppt;
        ScreenToClient(hStatusWnd,&pt);
        GetClientRect(hStatusWnd,&rc);
        if (!PtInRect(&rc,pt))
            return 0;

		if(pt.x < BITMAPX) {
			dwButton |= BUTTONMENU;
		}
		else if (pt.x < BITMAPCHINESEDES) {
			dwButton |= BUTTONPY;
		}
		else if (pt.x < BITMAPFULLDES) {
			dwButton |= BUTTONCHINESE;
		}
		else if (pt.x < BITMAPPUNCTDES) {
			dwButton |= BUTTONFULL;
		}
		else if (pt.x < BITMAPPUNCTDES + BITMAPX) {
			dwButton |= BUTTONPUNCT;
		}
	}
    return dwButton;
}

void PaintStatus( HWND hStatusWnd,HDC hDC,DWORD dwButton )
{

    HIMC hIMC;
    LPINPUTCONTEXT lpIMC;
    HDC hMemDC;
    HBITMAP hBmpOld;
    HWND hSvrWnd;

    hSvrWnd = (HWND)GetWindowLong(hStatusWnd,FIGWL_SVRWND);

    if (hIMC = (HIMC)GetWindowLong(hSvrWnd,IMMGWL_IMC))
    {
        lpIMC = ImmLockIMC(hIMC);
        hMemDC = CreateCompatibleDC(hDC);
        hBmpOld = SelectObject(hMemDC,hBmpStatus);

		if( (dwButton & BUTTONMENU) || !dwButton){
			if( dwButton & BUTTONPUSH)
				BitBlt(hDC,0,0,BITMAPX,BITMAPY,hMemDC,0,BITMAPY,SRCCOPY);
			else
				BitBlt(hDC,0,0,BITMAPX,BITMAPY,hMemDC,0,0,SRCCOPY);
		}

		if( (dwButton & BUTTONPY) || !dwButton){
			if( dwButton & BUTTONPUSH)
				BitBlt(hDC,BITMAPX,0,BITMAPX*2,BITMAPY,hMemDC,BITMAPX,BITMAPY,SRCCOPY);
			else
				BitBlt(hDC,BITMAPX,0,BITMAPX*2,BITMAPY,hMemDC,BITMAPX,0,SRCCOPY);
		}

		if( (dwButton & BUTTONCHINESE) || !dwButton){
			if(lpIMC->fOpen){
				if( dwButton & BUTTONPUSH)
					BitBlt(hDC,BITMAPCHINESEDES,0,BITMAPX,BITMAPY,hMemDC,BITMAPCHINESESRC,BITMAPY,SRCCOPY);
				else
					BitBlt(hDC,BITMAPCHINESEDES,0,BITMAPX,BITMAPY,hMemDC,BITMAPCHINESESRC,0,SRCCOPY);
			}
			else{
				if( dwButton & BUTTONPUSH)
					BitBlt(hDC,BITMAPCHINESEDES,0,BITMAPX,BITMAPY,hMemDC,BITMAPCHINESESRC + BITMAPX,BITMAPY,SRCCOPY);
				else
					BitBlt(hDC,BITMAPCHINESEDES,0,BITMAPX,BITMAPY,hMemDC,BITMAPCHINESESRC + BITMAPX,0,SRCCOPY);
			}
		}

		if( (dwButton & BUTTONFULL) || !dwButton){
			if(wConversionSet & CONVERSION_SET_FULLSHAPE) {
				if( dwButton & BUTTONPUSH)
					BitBlt(hDC,BITMAPFULLDES,0,BITMAPX,BITMAPY,hMemDC,BITMAPFULLSRC,BITMAPY,SRCCOPY);
				else
					BitBlt(hDC,BITMAPFULLDES,0,BITMAPX,BITMAPY,hMemDC,BITMAPFULLSRC,0,SRCCOPY);
			}
			else{
				if( dwButton & BUTTONPUSH)
					BitBlt(hDC,BITMAPFULLDES,0,BITMAPX,BITMAPY,hMemDC,BITMAPFULLSRC + BITMAPX,BITMAPY,SRCCOPY);
				else
					BitBlt(hDC,BITMAPFULLDES,0,BITMAPX,BITMAPY,hMemDC,BITMAPFULLSRC + BITMAPX,0,SRCCOPY);
			}
		}

		if( (dwButton & BUTTONPUNCT) || !dwButton){
			if(wConversionSet & CONVERSION_SET_PUNCT) {
				if( dwButton & BUTTONPUSH)
					BitBlt(hDC,BITMAPPUNCTDES,0,BITMAPX,BITMAPY,hMemDC,BITMAPPUNCTSRC,BITMAPY,SRCCOPY);
				else
					BitBlt(hDC,BITMAPPUNCTDES,0,BITMAPX,BITMAPY,hMemDC,BITMAPPUNCTSRC,0,SRCCOPY);
			}
			else{
				if( dwButton & BUTTONPUSH)
					BitBlt(hDC,BITMAPPUNCTDES,0,BITMAPX,BITMAPY,hMemDC,BITMAPPUNCTSRC + BITMAPX,BITMAPY,SRCCOPY);
				else
					BitBlt(hDC,BITMAPPUNCTDES,0,BITMAPX,BITMAPY,hMemDC,BITMAPPUNCTSRC + BITMAPX,0,SRCCOPY);
			}
		}

		SelectObject(hMemDC,hBmpOld);
        DeleteDC(hMemDC);
        ImmUnlockIMC(hIMC);
    }
}

void UpdateStatusWindow(LPUIEXTRA lpUIExtra)
{
    if (IsWindow(lpUIExtra->uiStatus.hWnd)) {
		InvalidateRect(lpUIExtra->uiStatus.hWnd,NULL,FALSE);
	}
}

BOOL WINAPI SetDialogProc(
			HWND hWnd ,
			UINT message ,
			WPARAM wParam ,
			LPARAM lParam )
{
	switch(message) {
	case WM_INITDIALOG:
		CheckDlgButton(hWnd,IDC_CHECKSORT,
			(wConversionSet & CONVERSION_SET_SORT)? BST_CHECKED :BST_UNCHECKED );
		CheckDlgButton(hWnd,IDC_CHECKFOLLOW,
			(wConversionSet & CONVERSION_SET_FOLLOW)? BST_CHECKED :BST_UNCHECKED );
		CheckDlgButton(hWnd,IDC_CHECKGBK,
			(wConversionSet & CONVERSION_SET_GBK)? BST_CHECKED :BST_UNCHECKED );
		CheckDlgButton(hWnd,IDC_CHECKFUZZYC,
			(wConversionSet & CONVERSION_SET_FUZZYC)? BST_CHECKED :BST_UNCHECKED );
		CheckDlgButton(hWnd,IDC_CHECKFUZZYN,
			(wConversionSet & CONVERSION_SET_FUZZYN)? BST_CHECKED :BST_UNCHECKED );
		CheckDlgButton(hWnd,IDC_CHECKFUZZYH,
			(wConversionSet & CONVERSION_SET_FUZZYH)? BST_CHECKED :BST_UNCHECKED );
		CheckDlgButton(hWnd,IDC_CHECKFUZZYB,
			(wConversionSet & CONVERSION_SET_FUZZYB)? BST_CHECKED :BST_UNCHECKED );
		CheckDlgButton(hWnd,IDC_CHECKSHAPE,
			(wConversionSet & CONVERSION_SET_SHAPE)? BST_CHECKED :BST_UNCHECKED );
		break;

	case WM_COMMAND:
		switch(wParam) {
		case IDC_CHECKFUZZYC:
			break;

		case IDC_CHECKFUZZYN:
			break;

		case IDC_CHECKFUZZYH:
			break;

		case IDC_CHECKSORT:
			break;

		case IDOK:
			if(IsDlgButtonChecked(hWnd,IDC_CHECKSORT) == BST_CHECKED)
				wConversionSet |= CONVERSION_SET_SORT;
			else
				wConversionSet &= ~CONVERSION_SET_SORT;

			if(IsDlgButtonChecked(hWnd,IDC_CHECKFOLLOW) == BST_CHECKED)
				wConversionSet |= CONVERSION_SET_FOLLOW;
			else
				wConversionSet &= ~CONVERSION_SET_FOLLOW;

			if(IsDlgButtonChecked(hWnd,IDC_CHECKGBK) == BST_CHECKED)
				wConversionSet |= CONVERSION_SET_GBK;
			else
				wConversionSet &= ~CONVERSION_SET_GBK;

			if(IsDlgButtonChecked(hWnd,IDC_CHECKFUZZYC) == BST_CHECKED)
				wConversionSet |= CONVERSION_SET_FUZZYC;
			else
				wConversionSet &= ~CONVERSION_SET_FUZZYC;

			if(IsDlgButtonChecked(hWnd,IDC_CHECKFUZZYN) == BST_CHECKED)
				wConversionSet |= CONVERSION_SET_FUZZYN;
			else
				wConversionSet &= ~CONVERSION_SET_FUZZYN;

			if(IsDlgButtonChecked(hWnd,IDC_CHECKFUZZYH) == BST_CHECKED)
				wConversionSet |= CONVERSION_SET_FUZZYH;
			else
				wConversionSet &= ~CONVERSION_SET_FUZZYH;

			if(IsDlgButtonChecked(hWnd,IDC_CHECKFUZZYB) == BST_CHECKED)
				wConversionSet |= CONVERSION_SET_FUZZYB;
			else
				wConversionSet &= ~CONVERSION_SET_FUZZYB;

			if(IsDlgButtonChecked(hWnd,IDC_CHECKSHAPE) == BST_CHECKED)
				wConversionSet |= CONVERSION_SET_SHAPE;
			else
				wConversionSet &= ~CONVERSION_SET_SHAPE;

			EndDialog(hWnd,TRUE);
			break;

		case IDCANCEL:
			EndDialog(hWnd,TRUE);
			break;

		default:
			return FALSE;
			break;
		}
		break;

	default:
		return FALSE;
		break;
	}
	return TRUE;
}

BOOL WINAPI AboutDialogProc(
			HWND hWnd ,
			UINT message ,
			WPARAM wParam ,
			LPARAM lParam )
{

	switch(message) {
	case WM_INITDIALOG:
		break;

	case WM_COMMAND:
		switch(wParam) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd,TRUE);
			break;
		default:
			return FALSE;
			break;
		}
		break;
	default:
		return FALSE;
		break;
	}
	return TRUE;
}