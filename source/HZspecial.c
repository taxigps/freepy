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

BOOL RepeatPreResult(HIMC hIMC)
{
    LPINPUTCONTEXT lpIMC;
    LPCOMPOSITIONSTRING lpCompStr;
    GENEMSG GnMsg;
	LPTSTR lpPreResultStr;

	wConversionMode = 0;

    lpIMC = ImmLockIMC(hIMC);
	lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);

	lpPreResultStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPreResultStr;
	if( _tcslen(lpPreResultStr) ){
		_tcscpy(GETLPRESULTSTR(lpCompStr),lpPreResultStr);
		lpCompStr->dwResultStrLen = _tcslen(lpPreResultStr);
		lpCompStr->dwCompStrLen = 0;

		GnMsg.msg = WM_IME_COMPOSITION;
		GnMsg.wParam = 0;
		GnMsg.lParam = GCS_RESULTSTR;
		GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);

		GnMsg.msg = WM_IME_ENDCOMPOSITION;
		GnMsg.wParam = 0;
		GnMsg.lParam = 0;
		GenerateMessage(hIMC,  lpdwCurTransKey,(LPGENEMSG)&GnMsg);

		ImmUnlockIMCC(lpIMC->hCompStr);
	    ImmUnlockIMC(hIMC);
		return TRUE;
	}

	ImmUnlockIMCC(lpIMC->hCompStr);
    ImmUnlockIMC(hIMC);
	return FALSE;
}

BOOL GeneratePunct(HIMC hIMC, WORD wCode)
{
    LPINPUTCONTEXT lpIMC;
    LPCOMPOSITIONSTRING lpCompStr;
	LPTSTR lpStr,lpConvStr,lpPreResultStr;
	WORD wHead;
	TCHAR cLastPreResultChar;

    lpIMC = ImmLockIMC(hIMC);
	lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
	lpCompStr->dwCompStrLen = 0;
	lpConvStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szConvCompStr;

	if(wConversionSet & CONVERSION_SET_PUNCT) {
		wHead = wCode - _T('!');
		lpStr = aPunct[wHead][0];
		if( _tcslen(lpStr) ){
			if( wCode == _T('.') || wCode == _T('/') ) {
				lpPreResultStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPreResultStr;
				cLastPreResultChar = *CharPrev(lpPreResultStr,lpPreResultStr + _tcslen(lpPreResultStr));
				if(cLastPreResultChar >= _T('!') && cLastPreResultChar <= _T('~') ) {
					*lpConvStr = (TCHAR)wCode;
					*(lpConvStr+1) = _T('\0');
				}
				else _tcscpy(lpConvStr,lpStr);
			}
			else _tcscpy(lpConvStr,lpStr);

			MakeResultString(hIMC,TRUE);
			ImmUnlockIMCC(lpIMC->hCompStr);
			ImmUnlockIMC(hIMC);
			return TRUE;
		}
	}
	else {
		*lpConvStr = (TCHAR)wCode;
		*(lpConvStr+1) = _T('\0');
		MakeResultString(hIMC,TRUE);
		ImmUnlockIMCC(lpIMC->hCompStr);
		ImmUnlockIMC(hIMC);
		return TRUE;
	}

	ImmUnlockIMCC(lpIMC->hCompStr);
    ImmUnlockIMC(hIMC);
	return FALSE;
}