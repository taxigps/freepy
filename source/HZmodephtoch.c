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

BOOL CharHandlePhToCh( HIMC hIMC,WORD wParam,LONG lParam)
{
	if( wParam >= _T('0') && wParam <= _T('9') ){
	    LPINPUTCONTEXT lpIMC;
		LPCANDIDATEINFO lpCandInfo;
		LPCANDIDATELIST lpCandList;
		LPCOMPOSITIONSTRING lpCompStr;
		WORD wIdx,wCount;
		TCHAR szCharArr[50][4];
		LPTSTR lpNextChar,lpStr,lpConvStr;
		GENEMSG GnMsg;
		int i;

		lpIMC = ImmLockIMC(hIMC);
		lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
		lpCandList = (LPCANDIDATELIST)((LPSTR)lpCandInfo  + lpCandInfo->dwOffset[0]);
		lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);

	    if( wParam == (WORD)_T('0') ) wIdx=9;
		else wIdx = wParam - _T('1');

		wIdx += (WORD)lpCandList->dwPageStart;

		if( wIdx >= 2 && wIdx < lpCandList->dwCount+2){
			wCount = 0;
			lpStr = GETLPCANDSTR(lpCandList,wIdx);
			lpNextChar = CharNext(lpStr);
			while( lpStr != lpNextChar ){
				memcpy(szCharArr[wCount],lpStr,lpNextChar-lpStr);
				szCharArr[wCount++][lpNextChar-lpStr] = _T('\0');
				lpStr = lpNextChar;
				lpNextChar = CharNext(lpStr);
			}
			if( !wCount ) goto my_exit;
			if( wCount == 1){
				lpConvStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szConvCompStr;
				_tcscpy(lpConvStr,szCharArr[0]);
				MakeResultString(hIMC,TRUE);
				goto my_exit;
			}
			if( wCount > 10 ) wCount = 10;
			lpCandList->dwCount = wCount;
			lpCandList->dwPageStart = 2;
			lpCandList->dwPageSize = 10;
			for (i=0;i<wCount;i++){
				_tcscpy(GETLPCANDSTR(lpCandList,i+2),szCharArr[i]);
			}
			GnMsg.msg = WM_IME_COMPOSITION;
			GnMsg.wParam = 0;
			GnMsg.lParam = GCS_COMPSTR;
			GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
		}
my_exit:
		ImmUnlockIMCC(lpIMC->hCandInfo);
		ImmUnlockIMCC(lpIMC->hCompStr);
		ImmUnlockIMC(hIMC);
	}
	return TRUE;
}
