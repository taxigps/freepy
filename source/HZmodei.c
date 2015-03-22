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
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>

static TCHAR CDigit[][4] = { _T("零"),_T("十"),_T("百"),_T("千"),_T("万"),_T("亿"),_T("兆") };
static TCHAR CNum[][4] = {_T("零"),_T("一"),_T("二"),_T("三"),_T("四"),_T("五"),_T("六"),_T("七"),_T("八"),_T("九"),_T("十")};
static TCHAR CData[][4] = { _T("年"),_T("月"),_T("日"),_T("时"),_T("分"),_T("秒")};
static TCHAR CWeek[][8] = { _T("星期日"),_T("星期一"),_T("星期二"),_T("星期三"),_T("星期四"),_T("星期五"),_T("星期六"),_T("上午"),_T("下午")};

void ChineseNumber(int i , LPTSTR lpStr)
{
	if( i<11 && i>= 0 ) {
		_tcscpy(lpStr,CNum[i]);
	}
	else if( i<20 ) {
		_tcscpy(lpStr,CNum[10]);
		_tcscat(lpStr,CNum[i-10]);
	}
	else if( i<100 ) {
		if( i%10 ) {
			_tcscpy(lpStr,CNum[i/10]);
			_tcscat(lpStr,CDigit[1]);
			_tcscat(lpStr,CNum[i%10]);
		}
		else {
			_tcscpy(lpStr,CNum[i/10]);
			_tcscat(lpStr,CDigit[1]);
		}
	}
	else if( i<100000 ) {
		int nRest,nDevide;
		BOOL fStart = FALSE;

		nDevide = 10000;
		nRest = i;

		*lpStr = _T('\0');
		while( nDevide ){
			i = nRest/nDevide;
			nRest %= nDevide;
			if( !i && fStart) _tcscat(lpStr,CNum[i]);
			else if ( i ) {
				fStart = TRUE;
				_tcscat(lpStr,CNum[i]);
			}
			nDevide /= 10;
		}
	}
	return;
}

BOOL CharHandleI( HIMC hIMC,WORD wParam,LONG lParam)
{
    LPINPUTCONTEXT lpIMC;
	LPCANDIDATEINFO lpCandInfo;
	LPCANDIDATELIST lpCandList;
	LPCOMPOSITIONSTRING lpCompStr;


    lpIMC = ImmLockIMC(hIMC);
	lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
	lpCandList = (LPCANDIDATELIST)((LPSTR)lpCandInfo  + lpCandInfo->dwOffset[0]);
	lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);

	if( !lpCandList->dwCount ){
		if( wParam == _T('d') || wParam == _T('t')){
			time_t ltime;
			struct tm *today;
			int nYear,nMon,nDay,nHour,nMin,nSec,nWeek;
			BOOL fPM = FALSE;
			LPTSTR lpStr;
			WORD wStrLen;

			time( &ltime );
			today = localtime( &ltime );
			nSec = today->tm_sec;
			nMin = today->tm_min;
			nHour =	today->tm_hour;
			nDay = today->tm_mday;
			nMon = today->tm_mon + 1;
			nYear =	today->tm_year + 1900;
			nWeek =	today->tm_wday;

			if( wParam == _T('d') ){
				lpStr = GETLPCANDSTR(lpCandList,2);
				_stprintf(lpStr,_T("%d"),nYear);
				_tcscat(lpStr,CData[0]);
				_stprintf(lpStr+_tcslen(lpStr),_T("%d"),nMon);
				_tcscat(lpStr,CData[1]);
				_stprintf(lpStr+_tcslen(lpStr),_T("%d"),nDay);
				_tcscat(lpStr,CData[2]);

				lpStr = GETLPCANDSTR(lpCandList,3);
				ChineseNumber(nYear,lpStr);
				_tcscat(lpStr,CData[0]);

				lpStr = GETLPCANDSTR(lpCandList,4);
				ChineseNumber(nMon,lpStr);
				_tcscat(lpStr,CData[1]);
				ChineseNumber(nDay,lpStr + _tcslen(lpStr));
				_tcscat(lpStr,CData[2]);

				lpStr = GETLPCANDSTR(lpCandList,5);
				_stprintf(lpStr,_T("%d.%d.%d"),nYear,nMon,nDay);

				lpStr = GETLPCANDSTR(lpCandList,6);
				_stprintf(lpStr,_T("%d/%d/%d"),nMon,nDay,nYear);

				lpStr = GETLPCANDSTR(lpCandList,7);
				_tcscpy(lpStr,CWeek[nWeek]);

				lpCandList->dwSelection = 0;
				lpCandList->dwCount = 6;
				lpCandList->dwPageStart = 2;
				lpCandList->dwPageSize = 0;
			}
			if(wParam == _T('t') ) {
				lpStr = GETLPCANDSTR(lpCandList,2);
				ChineseNumber(nHour,lpStr);
				_tcscat(lpStr,CData[3]);
				ChineseNumber(nMin,lpStr + _tcslen(lpStr));
				_tcscat(lpStr,CData[4]);
				ChineseNumber(nSec,lpStr + _tcslen(lpStr));
				_tcscat(lpStr,CData[5]);

				lpStr = GETLPCANDSTR(lpCandList,3);
				ChineseNumber(nHour,lpStr);
				_tcscat(lpStr,CData[3]);
				ChineseNumber(nMin,lpStr + _tcslen(lpStr));
				_tcscat(lpStr,CData[4]);


				lpStr = GETLPCANDSTR(lpCandList,4);
				_stprintf(lpStr,_T("%d"),nHour);
				_tcscat(lpStr,CData[3]);
				_stprintf(lpStr+_tcslen(lpStr),_T("%d"),nMin);
				_tcscat(lpStr,CData[4]);
				_stprintf(lpStr+_tcslen(lpStr),_T("%d"),nSec);
				_tcscat(lpStr,CData[5]);


				lpStr = GETLPCANDSTR(lpCandList,5);
				_stprintf(lpStr,_T("%d"),nHour);
				_tcscat(lpStr,CData[3]);
				_stprintf(lpStr+_tcslen(lpStr),_T("%d"),nMin);
				_tcscat(lpStr,CData[4]);

				lpStr = GETLPCANDSTR(lpCandList,6);
				_stprintf(lpStr,_T("%d:%d:%d"),nHour,nMin,nSec);

				lpStr = GETLPCANDSTR(lpCandList,7);
				_stprintf(lpStr,_T("%d:%d"),nHour,nMin);

				lpCandList->dwSelection = 0;
				lpCandList->dwCount = 6;
				lpCandList->dwPageStart = 2;
				lpCandList->dwPageSize = 0;

			}
			lpStr = GETLPCOMPSTR(lpCompStr);
			wStrLen = _tcslen(lpStr);
			*(lpStr + wStrLen) = (TCHAR)wParam;
			*(lpStr + wStrLen +1) = _T('\0');

			lpStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPaintCompStr;
			wStrLen = _tcslen(lpStr);
			*(lpStr + wStrLen) = (TCHAR)wParam;
			*(lpStr + wStrLen +1) = _T('\0');

			SelectForwardFromCand(hIMC,lpCandList);
		}
		else MessageBeep(0xFFFFFFFF );
	}
	else {
		if( wParam == _T('=') || wParam == _T('.') || wParam == _T('>')) {
			SelectForwardFromCand(hIMC,lpCandList);
		}
		else if( wParam == _T('-') || wParam == _T(',') || wParam == _T('<')) {
			SelectBackwardFromCand(hIMC,lpCandList);
		}

		if( wParam >= _T('0') && wParam <= _T('9') ){
			SelectCandFromCandlist(hIMC, wParam);
		}
	}

	ImmUnlockIMCC(lpIMC->hCompStr);
	ImmUnlockIMCC(lpIMC->hCandInfo);
	ImmUnlockIMC(hIMC);
	return TRUE;
}
