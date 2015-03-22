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

BOOL IsCompStr(HIMC hIMC)
{
    LPINPUTCONTEXT lpIMC;
    LPCOMPOSITIONSTRING lpCompStr;
    BOOL fRet = FALSE;

    if (!(lpIMC = ImmLockIMC(hIMC)))
        return FALSE;

    if (ImmGetIMCCSize(lpIMC->hCompStr) < sizeof (COMPOSITIONSTRING))
    {
        ImmUnlockIMC(hIMC);
        return FALSE;
    }

    lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);

    fRet = (lpCompStr->dwCompStrLen > 0);

    ImmUnlockIMCC(lpIMC->hCompStr);
    ImmUnlockIMC(hIMC);

    return fRet;
}

/**********************************************************************/
/*                                                                    */
/*      InitCompStr()                                                  */
/*                                                                    */
/**********************************************************************/
void InitCompStr(LPCOMPOSITIONSTRING lpCompStr)
{
	LPFREEPYCOMP lpFreePYComp;

	lpCompStr->dwSize = sizeof(MYCOMPSTR);

	lpCompStr->dwCompStrOffset = 
            (DWORD)((LPMYCOMPSTR)lpCompStr)->szCompStr - (DWORD)lpCompStr;
	lpCompStr->dwCompStrLen = 0;
	*GETLPCOMPSTR(lpCompStr) = _T('\0');

	lpCompStr->dwResultStrOffset = 
          (DWORD)((LPMYCOMPSTR)lpCompStr)->szResultStr - (DWORD)lpCompStr;
	lpCompStr->dwResultStrLen = 0;
	*GETLPRESULTSTR(lpCompStr) = _T('\0');

	lpCompStr->dwCursorPos = 0;

	lpFreePYComp= &(((LPMYCOMPSTR)lpCompStr)->FreePYComp);
	lpFreePYComp->szConvCompStr[0] = _T('\0');
	lpFreePYComp->szPaintCompStr[0] = _T('\0');
	lpFreePYComp->szPreResultStr[0] = _T('\0');
	lpFreePYComp->wEditCaret = 0;
	lpFreePYComp->wUnConvPos = 0;
	lpFreePYComp->wPYArrayLen = 0;
	lpFreePYComp->wPYArrayCurPos = 0;
}

void ClearCompStr(LPCOMPOSITIONSTRING lpCompStr)
{
	LPFREEPYCOMP lpFreePYComp;

	lpCompStr->dwCursorPos = 0;
	lpCompStr->dwCompStrLen = 0;
	((LPMYCOMPSTR)lpCompStr)->szCompStr[0] = _T('\0');
	lpCompStr->dwResultStrLen = 0;
	((LPMYCOMPSTR)lpCompStr)->szResultStr[0] = _T('\0');

	lpFreePYComp= &(((LPMYCOMPSTR)lpCompStr)->FreePYComp);
	lpFreePYComp->szConvCompStr[0] = _T('\0');
	lpFreePYComp->szPaintCompStr[0] = _T('\0');
	lpFreePYComp->wEditCaret = 0;
	lpFreePYComp->wUnConvPos = 0;
	lpFreePYComp->wPYArrayLen = 0;
	lpFreePYComp->wPYArrayCurPos = 0;
}

BOOL IsCandidate(HIMC hIMC)
{
    LPINPUTCONTEXT lpIMC;
    LPCANDIDATEINFO lpCandInfo;
	LPCANDIDATELIST lpCandList;
    BOOL fRet = FALSE;

    if (!(lpIMC = ImmLockIMC(hIMC)))
        return FALSE;

    if (ImmGetIMCCSize(lpIMC->hCandInfo) < sizeof (CANDIDATEINFO)){
		ImmUnlockIMC(hIMC);
        return FALSE;
	}

    lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
	lpCandList = (LPCANDIDATELIST)((LPBYTE)lpCandInfo  + lpCandInfo->dwOffset[0]);

    fRet = (lpCandList->dwCount > 0);

    ImmUnlockIMCC(lpIMC->hCandInfo);
	ImmUnlockIMC(hIMC);
    return fRet;
}

void InitCandInfo(LPCANDIDATEINFO lpCandInfo)
{
	LPCANDIDATELIST lpCandList;
	LPFREEPYCAND lpFreePYCand;
	int i;

	lpCandInfo->dwSize = sizeof(MYCAND);
	lpCandInfo->dwCount = 1;
	lpCandInfo->dwOffset[0] = 
			(DWORD)(&(((LPMYCAND)lpCandInfo)->cl)) - (DWORD)lpCandInfo;

	lpCandList = (LPCANDIDATELIST)((LPBYTE)lpCandInfo  + lpCandInfo->dwOffset[0]);
	for(i=0;i<MAXCANDSTRNUM;i++) {
		lpCandList->dwOffset[i] = 
                   (DWORD)((LPMYCAND)lpCandInfo)->szCandStr[i] - (DWORD)lpCandList;
	}
	lpCandList->dwSize = sizeof(MYCAND) - sizeof(CANDIDATEINFO); 
	lpCandList->dwCount = 0L;
	lpCandList->dwSelection = 0L;
	lpCandList->dwPageStart = 0L;
	lpCandList->dwPageSize = 0L;

	lpFreePYCand= &(((LPMYCAND)lpCandInfo)->FreePYCand);
	lpFreePYCand->wPhraseNum = 0;
	lpFreePYCand->wHZNum = 0;
	lpFreePYCand->wKeyLen = 0;
	lpFreePYCand->abKey[0] = 0;
}

void ClearCandidate(LPCANDIDATEINFO lpCandInfo)
{
	LPFREEPYCAND lpFreePYCand;

    ((LPMYCAND)lpCandInfo)->cl.dwCount =0L;
    ((LPMYCAND)lpCandInfo)->cl.dwSelection =0L;
    ((LPMYCAND)lpCandInfo)->cl.dwPageStart =0L;
    ((LPMYCAND)lpCandInfo)->cl.dwPageSize =0L;

	lpFreePYCand= &(((LPMYCAND)lpCandInfo)->FreePYCand);
	lpFreePYCand->wPhraseNum = 0;
	lpFreePYCand->wHZNum = 0;
	lpFreePYCand->wKeyLen = 0;
	lpFreePYCand->abKey[0] = 0;
}

HKL GetMyHKL()
{
    DWORD dwSize;
    DWORD dwi;
    HKL hKL = 0;
    HKL *lphkl;

    dwSize = GetKeyboardLayoutList(0, NULL);

    lphkl = (HKL *)GlobalAlloc(GPTR, dwSize * sizeof(DWORD));

    if (!lphkl)
        return NULL;

    GetKeyboardLayoutList(dwSize, lphkl);


    for (dwi = 0; dwi < dwSize; dwi++)
    {
        char szFile[32];
        HKL hKLTemp = *(lphkl + dwi);
        ImmGetIMEFileName(hKLTemp, szFile, sizeof(szFile));

        if (!_tcsicmp(szFile, FREEPYFILENAME))
        {
             hKL = hKLTemp;
             goto exit;
        }
    }
exit:

    GlobalFree((HANDLE)lphkl);
    return hKL;
}
/*****************************************************************************
*                                                                            *
* UpdateIndicIcon( hIMC )                                                    *
*                                                                            *
*****************************************************************************/
void UpdateIndicIcon(HIMC hIMC)
{
    HWND hwndIndicate;
    BOOL fOpen = FALSE;
    LPINPUTCONTEXT lpIMC;

    if (!hFreePYKL)
    {
       hFreePYKL = GetMyHKL();
       if (!hFreePYKL)
           return;
    }

    hwndIndicate = FindWindow(INDICATOR_CLASS, NULL);

    if (hIMC)
    {
        lpIMC = ImmLockIMC(hIMC);
        if (lpIMC)
        {
            fOpen = lpIMC->fOpen;
            ImmUnlockIMC(hIMC);
        }
    }

    if (IsWindow(hwndIndicate))
    {
        ATOM atomTip;

        atomTip = GlobalAddAtom(ATOMTIP);
        PostMessage(hwndIndicate, INDICM_SETIMEICON, 
                    fOpen ? 1 : (-1), (LPARAM)hFreePYKL);
        PostMessage(hwndIndicate, INDICM_SETIMETOOLTIPS, 
                    fOpen ? atomTip : (-1), (LPARAM)hFreePYKL);
        PostMessage(hwndIndicate, INDICM_REMOVEDEFAULTMENUITEMS, 
                    // fOpen ? (RDMI_LEFT | RDMI_RIGHT) : 0, (LPARAM)hMyKL);
                    fOpen ? (RDMI_LEFT) : 0, (LPARAM)hFreePYKL);
    }
}

/**********************************************************************/
/*                                                                    */
/* MyIsIMEMessage(message)                                            */
/*                                                                    */
/* Any UI window should not pass the IME messages to DefWindowProc.   */
/*                                                                    */
/**********************************************************************/
BOOL MyIsIMEMessage(UINT message)
{
    switch(message)
    {
            case WM_IME_STARTCOMPOSITION:
            case WM_IME_ENDCOMPOSITION:
            case WM_IME_COMPOSITION:
            case WM_IME_NOTIFY:
            case WM_IME_SETCONTEXT:
            case WM_IME_CONTROL:
            case WM_IME_COMPOSITIONFULL:
            case WM_IME_SELECT:
            case WM_IME_CHAR:
                return TRUE;
    }

    return FALSE;
}

BOOL MakeResultString( HIMC hIMC,BOOL fFlag)
{
    GENEMSG GnMsg;
    LPCOMPOSITIONSTRING lpCompStr;
    LPINPUTCONTEXT lpIMC;
	LPTSTR lpConvStr,lpPreResultStr;
	LPCANDIDATEINFO lpCandInfo;
	LPCANDIDATELIST lpCandList;
/*
    if (!IsCompStr(hIMC))
        return FALSE;
*/	
	wConversionMode = 0;

    lpIMC = ImmLockIMC(hIMC);
    lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
	lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
	lpCandList = (LPCANDIDATELIST)((LPSTR)lpCandInfo  + lpCandInfo->dwOffset[0]);

	if(fFlag) {
		lpConvStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szConvCompStr;
		lpPreResultStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPreResultStr;
		_tcscpy(GETLPRESULTSTR(lpCompStr),lpConvStr);
		_tcscpy(lpPreResultStr,lpConvStr);
		lpCompStr->dwResultStrLen = _tcslen(lpConvStr);
	}
	else{
		*GETLPRESULTSTR(lpCompStr) = _T('\0');
		lpCompStr->dwResultStrLen = 0;
	}

	lpCandList->dwCount = 0;
    lpCompStr->dwCompStrLen = 0;

	_tcscpy(GETLPCANDSTR(lpCandList,0),_T(""));
	_tcscpy(GETLPCANDSTR(lpCandList,1),_T(""));
	
	ImmUnlockIMCC(lpIMC->hCompStr);
	ImmUnlockIMCC(lpIMC->hCandInfo);
	
	GnMsg.msg = WM_IME_COMPOSITION;
	GnMsg.wParam = 0;
	GnMsg.lParam = GCS_RESULTSTR;
	GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
	
	GnMsg.msg = WM_IME_ENDCOMPOSITION;
	GnMsg.wParam = 0;
	GnMsg.lParam = 0;
	GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
	
    ImmUnlockIMC(hIMC);
    return TRUE;
}

/**********************************************************************/
/*                                                                    */
/*      GenerateMessage()                                             */
/*                                                                    */
/*      Update the transrate key buffer.                              */
/*                                                                    */
/**********************************************************************/
BOOL GenerateMessage(HIMC hIMC, LPDWORD lpdwTransKey,LPGENEMSG lpGeneMsg)
{
    LPINPUTCONTEXT lpIMC;

	if( (lpIMC = ImmLockIMC(hIMC)) == NULL ) return FALSE;
	
    if (lpdwTransKey){
		ImmUnlockIMC(hIMC);
        return GenerateMessageToTransKey(lpdwTransKey,lpGeneMsg);
	}
    
    if (IsWindow(lpIMC->hWnd))
    {
        LPDWORD lpdw;
        if (!(lpIMC->hMsgBuf = ImmReSizeIMCC(lpIMC->hMsgBuf,
			sizeof(DWORD) * (lpIMC->dwNumMsgBuf +1) * 3)))
            return FALSE;
		
        if (!(lpdw = (LPDWORD)ImmLockIMCC(lpIMC->hMsgBuf)))
            return FALSE;
		
        lpdw += (lpIMC->dwNumMsgBuf) * 3;
        *((LPGENEMSG)lpdw) = *lpGeneMsg;
        ImmUnlockIMCC(lpIMC->hMsgBuf);
        lpIMC->dwNumMsgBuf++;
		
        ImmGenerateMessage(hIMC);
    }
	ImmUnlockIMC(hIMC);
    return TRUE;
}

/**********************************************************************/
/*      GenerateMessageToTransKey()                                   */
/*                                                                    */
/*      Update the transrate key buffer.                              */
/**********************************************************************/
BOOL GenerateMessageToTransKey(LPDWORD lpdwTransKey,LPGENEMSG lpGeneMsg)
{
	LPDWORD lpdwTemp;
	
    uNumTransKey++;
    if (uNumTransKey >= (UINT)*lpdwTransKey)
    {
        fOverTransKey = TRUE;
        return FALSE;
    }
	
	lpdwTemp = (LPDWORD)lpdwTransKey + 1 + (uNumTransKey - 1)*3;
	*(lpdwTemp++) = lpGeneMsg->msg;
	*(lpdwTemp++) = lpGeneMsg->wParam;
	*(lpdwTemp++) = lpGeneMsg->lParam;
    return TRUE;
}

/**********************************************************************/
/*                                                                    */
/* DrawUIBorder()                                                     */
/*                                                                    */
/* When draging the child window, this function draws the border.     */
/*                                                                    */
/**********************************************************************/
void DrawUIBorder( LPRECT lprc )
{
    HDC hDC;
    int sbx, sby;
	
    hDC = CreateDC( "DISPLAY", NULL, NULL, NULL );
    SelectObject( hDC, GetStockObject( GRAY_BRUSH ) );
    sbx = GetSystemMetrics( SM_CXBORDER );
    sby = GetSystemMetrics( SM_CYBORDER );
    PatBlt( hDC, lprc->left, 
		lprc->top, 
		lprc->right - lprc->left-sbx, 
		sby, PATINVERT );
    PatBlt( hDC, lprc->right - sbx, 
		lprc->top, 
		sbx, 
		lprc->bottom - lprc->top-sby, PATINVERT );
    PatBlt( hDC, lprc->right, 
		lprc->bottom-sby, 
		-(lprc->right - lprc->left-sbx), 
		sby, PATINVERT );
    PatBlt( hDC, lprc->left, 
		lprc->bottom, 
		sbx, 
		-(lprc->bottom - lprc->top-sby), PATINVERT );
    DeleteDC( hDC );
}

void DragUI( HWND hWnd, HWND hWnd1,UINT message, WPARAM wParam, LPARAM lParam,BOOL fIsCompWnd)
{
    POINT     pt;
    static    POINT ptdif,ptdif1;
    static    RECT drc,drc1;
    static    SIZE sz,sz1;
    DWORD     dwT;
	
    switch (message)
    {
	case WM_SETCURSOR:
		if ( HIWORD(lParam) == WM_LBUTTONDOWN
			|| HIWORD(lParam) == WM_RBUTTONDOWN ) 
		{
			GetCursorPos( &pt );
			SetCapture(hWnd);
			GetWindowRect(hWnd,&drc);
			ptdif.x = pt.x - drc.left;
			ptdif.y = pt.y - drc.top;
			sz.cx = drc.right - drc.left;
			sz.cy = drc.bottom - drc.top;

			if (IsWindow(hWnd1)) {
				GetWindowRect(hWnd1,&drc1);
				ptdif1.x = pt.x - drc1.left;
				ptdif1.y = pt.y - drc1.top;
				sz1.cx = drc1.right - drc1.left;
				sz1.cy = drc1.bottom - drc1.top;
			}

			SetWindowLong(hWnd,FIGWL_MOUSE,FIM_CAPUTURED);
		}
		break;
		
	case WM_MOUSEMOVE:
		dwT = GetWindowLong(hWnd,FIGWL_MOUSE);
		if (dwT & FIM_MOVED)
		{
			DrawUIBorder(&drc);
			if (IsWindow(hWnd1)) DrawUIBorder(&drc1);
			GetCursorPos( &pt );
			drc.left   = pt.x - ptdif.x;
			drc.top    = pt.y - ptdif.y;
			drc.right  = drc.left + sz.cx;
			drc.bottom = drc.top + sz.cy;
			if (IsWindow(hWnd1)) {
				drc1.left   = pt.x - ptdif1.x;
				drc1.top    = pt.y - ptdif1.y;
				drc1.right  = drc1.left + sz1.cx;
				drc1.bottom = drc1.top + sz1.cy;
			}
			DrawUIBorder(&drc);
			if (IsWindow(hWnd1)) DrawUIBorder(&drc1);
		}
		else if (dwT & FIM_CAPUTURED)
		{
			DrawUIBorder(&drc);
			if (IsWindow(hWnd1)) DrawUIBorder(&drc1);
			SetWindowLong(hWnd,FIGWL_MOUSE,dwT | FIM_MOVED);
		}
		break;
		
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		dwT = GetWindowLong(hWnd,FIGWL_MOUSE);
		
		if (dwT & FIM_CAPUTURED)
		{
			ReleaseCapture();
			if (dwT & FIM_MOVED)
			{
				DrawUIBorder(&drc);
				if (IsWindow(hWnd1)) DrawUIBorder(&drc1);
				GetCursorPos( &pt );
				MoveWindow(hWnd,pt.x - ptdif.x,
					pt.y - ptdif.y,
					sz.cx,
					sz.cy,TRUE);

				if(fIsCompWnd) {
					HWND hUIWnd;
					LPARAM mylParam;

					*((LPWORD)(&mylParam)) = (WORD)(pt.x - ptdif.x);
					*((LPWORD)(&mylParam)+1) = (WORD)(pt.y - ptdif.y);

					hUIWnd = (HWND)GetWindowLong(hWnd,FIGWL_SVRWND);
					if (IsWindow(hUIWnd))
						SendMessage(hUIWnd,WM_UI_COMPMOVE,0,mylParam);
				}

				if (IsWindow(hWnd1)) {
					MoveWindow(hWnd1,pt.x - ptdif1.x,
						pt.y - ptdif1.y,
						sz1.cx,
						sz1.cy,TRUE);
				}
			}
		}
		break;
    }
}


void SelectCandFromCandlist( HIMC hIMC, WORD wParam)
{
	if( wParam >= _T('0') && wParam <= _T('9') ){
		DWORD dwIdx;
		LPTSTR lpConvStr;
	    LPINPUTCONTEXT lpIMC;
		LPCANDIDATEINFO lpCandInfo;
		LPCANDIDATELIST lpCandList;
		LPCOMPOSITIONSTRING lpCompStr;

		lpIMC = ImmLockIMC(hIMC);
		lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
		lpCandList = (LPCANDIDATELIST)((LPSTR)lpCandInfo  + lpCandInfo->dwOffset[0]);
		lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);

	    if( wParam == (WORD)_T('0') ) dwIdx=9;
		else dwIdx = wParam - _T('1');
		if(dwIdx < lpCandList->dwPageSize ) {
			dwIdx += lpCandList->dwPageStart;
			if( dwIdx < (lpCandList->dwCount + 2) && lpCandList->dwCount){
				lpConvStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szConvCompStr;
				_tcscpy(lpConvStr,GETLPCANDSTR(lpCandList,dwIdx));
				MakeResultString(hIMC,TRUE);
			}
		}
		ImmUnlockIMCC(lpIMC->hCompStr);
		ImmUnlockIMCC(lpIMC->hCandInfo);
		ImmUnlockIMC(hIMC);
	}
	return;
}

void DeleteCharBackward(HIMC hIMC,WORD wParam)
{
	if(wConversionMode & CONVERSION_MODE_PHRASETOCHAR){
		MakeResultString(hIMC,FALSE);
	}
	else if( wConversionMode & CONVERSION_MODE_I   || 
			 wConversionMode & CONVERSION_MODE_U   ||
			 wConversionMode & CONVERSION_MODE_V ) {

	    LPINPUTCONTEXT lpIMC;
		LPCANDIDATEINFO lpCandInfo;
		LPCANDIDATELIST lpCandList;
		LPCOMPOSITIONSTRING lpCompStr;
		GENEMSG GnMsg;
		LPTSTR lpStr;

		lpIMC = ImmLockIMC(hIMC);
		lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
		lpCandList = (LPCANDIDATELIST)((LPSTR)lpCandInfo  + lpCandInfo->dwOffset[0]);
		lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);

		if( !lpCandList->dwCount ) MakeResultString(hIMC,FALSE);
		else{
			lpCandList->dwCount = 0;

			lpStr = GETLPCOMPSTR(lpCompStr);
			lpStr = CharPrev(lpStr,lpStr + _tcslen(lpStr));
			*lpStr= _T('\0');

			lpStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPaintCompStr;
			lpStr = CharPrev(lpStr,lpStr + _tcslen(lpStr));
			*lpStr= _T('\0');

			GnMsg.msg = WM_IME_COMPOSITION;
			GnMsg.wParam = 0;
			GnMsg.lParam = GCS_COMPSTR;
			GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
		}
		ImmUnlockIMCC(lpIMC->hCompStr);
		ImmUnlockIMCC(lpIMC->hCandInfo);
		ImmUnlockIMC(hIMC);
	}
	else AddChar(hIMC,wParam,EDIT_BACK);
	return;
}

void DeleteCharForward(HIMC hIMC,WORD wParam)
{
	AddChar(hIMC,wParam,EDIT_DELETE);
	return;
}

void SpaceSelect(HIMC hIMC,WORD wParam)
{
	WORD wCode;
	wCode = (WORD)_T('1');

	if(wConversionMode & CONVERSION_MODE_PHRASETOCHAR)
		CharHandlePhToCh( hIMC,wCode,0);
	else if(	wConversionMode & CONVERSION_MODE_I   || 
				wConversionMode & CONVERSION_MODE_U   ||
				wConversionMode & CONVERSION_MODE_V ) {

		SelectCandFromCandlist( hIMC, wCode);
	}
	else
		SelectCand(hIMC,wCode);
	return;
}

void SelectForwardFromCand(HIMC hIMC,LPCANDIDATELIST lpCandList)
{
	HDC hDC;
	TCHAR szTotal[200],szTemp[100];
	SIZE sz;
	WORD wPos;
	GENEMSG GnMsg;

	if(wConversionSet & CONVERSION_SET_SHAPE) {
		wPos = (WORD) lpCandList->dwSelection;
		if(wPos < lpCandList->dwCount) {
			hDC = GetDC(NULL);
			lpCandList->dwPageStart = wPos + 2;
			_tcscpy(szTotal,"<>");
			lpCandList->dwPageSize = 0;
			while(wPos < lpCandList->dwCount && lpCandList->dwPageSize < CAND_PAGESIZE) {
				_stprintf(szTemp,"%d%s ",1,GETLPCANDSTR(lpCandList,wPos+2));
				_tcscat(szTotal,szTemp);
				GetTextExtentPoint(hDC,szTotal,_tcslen(szTotal),&sz);
				if(sz.cx > sizeCand[1].cx) {
					break;
				}
				(lpCandList->dwPageSize)++;
				wPos++;
			}
			ReleaseDC(NULL,hDC);
			lpCandList->dwSelection = wPos;
			if(lpCandList->dwPageStart > 2)
				_tcscpy(GETLPCANDSTR(lpCandList,0),_T("<"));
			else
				_tcscpy(GETLPCANDSTR(lpCandList,0),_T(""));
			if(wPos < lpCandList->dwCount)
				_tcscpy(GETLPCANDSTR(lpCandList,1),_T(">"));
			else
				_tcscpy(GETLPCANDSTR(lpCandList,1),_T(""));

			GnMsg.msg = WM_IME_COMPOSITION;
			GnMsg.wParam = 0;
			GnMsg.lParam = GCS_COMPSTR;
			GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
		}
		else MessageBeep(0xFFFFFFFF );
	}
	else{
		if( lpCandList->dwPageStart + lpCandList->dwPageSize -2 < 
			lpCandList->dwCount) {
			lpCandList->dwPageStart += lpCandList->dwPageSize;
			lpCandList->dwPageSize = CAND_PAGESIZE;
	
			GnMsg.msg = WM_IME_COMPOSITION;
			GnMsg.wParam = 0;
			GnMsg.lParam = GCS_COMPSTR;
			GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
		}
		else MessageBeep(0xFFFFFFFF );
	}
}

void SelectBackwardFromCand(HIMC hIMC,LPCANDIDATELIST lpCandList)
{
	HDC hDC;
	TCHAR szTotal[200],szTemp[100];
	SIZE sz;
	WORD wCount;
	GENEMSG GnMsg;

	if(lpCandList->dwPageStart > 2) {
		if(wConversionSet & CONVERSION_SET_SHAPE) {
			hDC = GetDC(NULL);
			lpCandList->dwSelection = (WORD)lpCandList->dwPageStart - 2;
			wCount = (WORD)lpCandList->dwPageStart - 2;
			_tcscpy(szTotal,"<>");
			lpCandList->dwPageSize = 0;
			while(wCount > 0 && lpCandList->dwPageSize < CAND_PAGESIZE) {
				_stprintf(szTemp,"%d%s ",1,GETLPCANDSTR(lpCandList,wCount+2));
				_tcscat(szTotal,szTemp);
				GetTextExtentPoint(hDC,szTotal,_tcslen(szTotal),&sz);
				if(sz.cx > sizeCand[1].cx) {
					break;
				}
				(lpCandList->dwPageSize)++;
				wCount--;
			}
			ReleaseDC(NULL,hDC);
			lpCandList->dwPageStart -= (lpCandList->dwSelection - wCount);

			if(lpCandList->dwPageStart > 2)
				_tcscpy(GETLPCANDSTR(lpCandList,0),_T("<"));
			else
				_tcscpy(GETLPCANDSTR(lpCandList,0),_T(""));

			if(lpCandList->dwSelection < lpCandList->dwCount)
				_tcscpy(GETLPCANDSTR(lpCandList,1),_T(">"));
			else
				_tcscpy(GETLPCANDSTR(lpCandList,1),_T(""));

		}
		else {

			if( (int)(lpCandList->dwPageStart) - (int)(lpCandList->dwPageSize)  >= 1){ 
				lpCandList->dwPageStart -= lpCandList->dwPageSize;
			}
		}
		GnMsg.msg = WM_IME_COMPOSITION;
		GnMsg.wParam = 0;
		GnMsg.lParam = GCS_COMPSTR;
		GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
	}
	else MessageBeep(0xFFFFFFFF );
}

void SetPaintColor(HDC hDC, WORD wMode)
{
	if(wMode & CONVERSION_MODE_PHRASETOCHAR) {
		SetTextColor(hDC,RGB(0,0,128));
	}
	else if(wMode & CONVERSION_MODE_U) {
		SetTextColor(hDC,RGB(128,0,0));
	}
	else if(wMode & CONVERSION_MODE_I) {
		SetTextColor(hDC,RGB(0,128,128));
	}
	else if(wMode & CONVERSION_MODE_V) {
		SetTextColor(hDC,RGB(128,0,128));
	}
	else{
		SetTextColor(hDC,RGB(0,0,0));
	}
}

BOOL WINAPI ConfigDialogProc(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam )
{
	HKEY hKey,hChildKey;
	DWORD dwConvSet;
	DWORD dwData = sizeof(DWORD);
	DWORD dwDisposition;

	switch(message) {
	case WM_INITDIALOG:
		if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			_T("Software\\freepy"),
			0,
			KEY_READ,
			&hKey) == ERROR_SUCCESS ){
			if( RegQueryValueEx( hKey,
								_T("ConversionSet"),
								NULL,
								NULL,
								(LPBYTE)&dwConvSet,
								&dwData) == ERROR_SUCCESS ){

				CheckDlgButton(hWnd,CONFIGSORT,
						(dwConvSet & CONVERSION_SET_SORT)? BST_CHECKED :BST_UNCHECKED );
				CheckDlgButton(hWnd,CONFIGPUNCT,
						(dwConvSet & CONVERSION_SET_PUNCT)? BST_CHECKED :BST_UNCHECKED );
				CheckDlgButton(hWnd,CONFIGFULLSHAPE,
						(dwConvSet & CONVERSION_SET_FULLSHAPE)? BST_CHECKED :BST_UNCHECKED );
				CheckDlgButton(hWnd,CONFIGFOLLOW,
						(dwConvSet & CONVERSION_SET_FOLLOW)? BST_CHECKED :BST_UNCHECKED );
				CheckDlgButton(hWnd,CONFIGGBK,
						(dwConvSet & CONVERSION_SET_GBK)? BST_CHECKED :BST_UNCHECKED );
				CheckDlgButton(hWnd,CONFIGFUZZYC,
						(dwConvSet & CONVERSION_SET_FUZZYC)? BST_CHECKED :BST_UNCHECKED );
				CheckDlgButton(hWnd,CONFIGFUZZYN,
						(dwConvSet & CONVERSION_SET_FUZZYN)? BST_CHECKED :BST_UNCHECKED );
				CheckDlgButton(hWnd,CONFIGFUZZYH,
						(dwConvSet & CONVERSION_SET_FUZZYH)? BST_CHECKED :BST_UNCHECKED );
				CheckDlgButton(hWnd,CONFIGFUZZYB,
						(dwConvSet & CONVERSION_SET_FUZZYB)? BST_CHECKED :BST_UNCHECKED );
				CheckDlgButton(hWnd,CONFIGSHAPE,
						(dwConvSet & CONVERSION_SET_SHAPE)? BST_CHECKED :BST_UNCHECKED );

			}
			RegCloseKey(hKey);
		}
		break;

	case WM_COMMAND:
		switch(wParam) {
		case CONFIGFUZZYC:
			break;

		case CONFIGFUZZYN:
			break;

		case CONFIGFUZZYH:
			break;

		case CONFIGSORT:
			break;

		case CONFIGPUNCT:
			break;

		case CONFIGFULLSHAPE:
			break;

		case IDOK:
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
					
					dwConvSet = 0;
					if(IsDlgButtonChecked(hWnd,CONFIGSORT) == BST_CHECKED)
						dwConvSet |= CONVERSION_SET_SORT;
					else
						dwConvSet &= ~CONVERSION_SET_SORT;
					
					if(IsDlgButtonChecked(hWnd,CONFIGPUNCT) == BST_CHECKED)
						dwConvSet |= CONVERSION_SET_PUNCT;
					else
						dwConvSet &= ~CONVERSION_SET_PUNCT;
					
					if(IsDlgButtonChecked(hWnd,CONFIGFULLSHAPE) == BST_CHECKED)
						dwConvSet |= CONVERSION_SET_FULLSHAPE;
					else
						dwConvSet &= ~CONVERSION_SET_FULLSHAPE;

					if(IsDlgButtonChecked(hWnd,CONFIGFOLLOW) == BST_CHECKED)
						dwConvSet |= CONVERSION_SET_FOLLOW;
					else
						dwConvSet &= ~CONVERSION_SET_FOLLOW;

					if(IsDlgButtonChecked(hWnd,CONFIGGBK) == BST_CHECKED)
						dwConvSet |= CONVERSION_SET_GBK;
					else
						dwConvSet &= ~CONVERSION_SET_GBK;
					
					if(IsDlgButtonChecked(hWnd,CONFIGFUZZYC) == BST_CHECKED)
						dwConvSet |= CONVERSION_SET_FUZZYC;
					else
						dwConvSet &= ~CONVERSION_SET_FUZZYC;
					
					if(IsDlgButtonChecked(hWnd,CONFIGFUZZYN) == BST_CHECKED)
						dwConvSet |= CONVERSION_SET_FUZZYN;
					else
						dwConvSet &= ~CONVERSION_SET_FUZZYN;
					
					if(IsDlgButtonChecked(hWnd,CONFIGFUZZYH) == BST_CHECKED)
						dwConvSet |= CONVERSION_SET_FUZZYH;
					else
						dwConvSet &= ~CONVERSION_SET_FUZZYH;

					if(IsDlgButtonChecked(hWnd,CONFIGFUZZYB) == BST_CHECKED)
						dwConvSet |= CONVERSION_SET_FUZZYB;
					else
						dwConvSet &= ~CONVERSION_SET_FUZZYB;

					if(IsDlgButtonChecked(hWnd,CONFIGSHAPE) == BST_CHECKED)
						dwConvSet |= CONVERSION_SET_SHAPE;
					else
						dwConvSet &= ~CONVERSION_SET_SHAPE;

					wConversionSet = (WORD)dwConvSet;
					
					RegSetValueEx(hChildKey,
						_T("ConversionSet"),
						0,
						REG_DWORD,
						(LPBYTE)&dwConvSet,
						sizeof(DWORD));
					RegCloseKey(hChildKey);
				}
				
				RegCloseKey(hKey);
			}
			
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
