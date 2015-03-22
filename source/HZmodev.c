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

static BYTE bPunctChar[9][192] = {
	//A1
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //40-4F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //50-5F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //60-6F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //70-7F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //80-8F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //90-9F
	  0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //A0-AF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //B0-BF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //C0-CF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //D0-DF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //E0-EF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 //F0-FF
	},
	//A2
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //40-4F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //50-5F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //60-6F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //70-7F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //80-8F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //90-9F
	  0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0, //A0-AF
	  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //B0-BF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //C0-CF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //D0-DF
	  1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0, //E0-EF
	  0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0 //F0-FF
	},
	//A3
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //40-4F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //50-5F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //60-6F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //70-7F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //80-8F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //90-9F
	  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //A0-AF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //B0-BF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //C0-CF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //D0-DF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //E0-EF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0 //F0-FF
	},
	//A4
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //40-4F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //50-5F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //60-6F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //70-7F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //80-8F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //90-9F
	  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //A0-AF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //B0-BF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //C0-CF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //D0-DF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //E0-EF
	  1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0 //F0-FF
	},
	//A5
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //40-4F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //50-5F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //60-6F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //70-7F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //80-8F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //90-9F
	  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //A0-AF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //B0-BF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //C0-CF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //D0-DF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //E0-EF
	  1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0 //F0-FF
	},
	//A6
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //40-4F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //50-5F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //60-6F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //70-7F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //80-8F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //90-9F
	  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //A0-AF
	  1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0, //B0-BF
	  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //C0-CF
	  1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0, //D0-DF
	  1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1, //E0-EF
	  1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0 //F0-FF
	},
	//A7
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //40-4F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //50-5F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //60-6F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //70-7F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //80-8F
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //90-9F
	  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //A0-AF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //B0-BF
	  1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //C0-CF
	  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //D0-DF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //E0-EF
	  1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 //F0-FF
	},
	//A8
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //40-4F
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //50-5F
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //60-6F
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0, //70-7F
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //80-8F
	  1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0, //90-9F
	  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //A0-AF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //B0-BF
	  1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1, //C0-CF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //D0-DF
	  1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0, //E0-EF
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 //F0-FF
	},
	//A9
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //40-4F
	  1,1,1,1,1,1,1,1,0,1,1,0,1,0,0,0, //50-5F
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //60-6F
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0, //70-7F
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //80-8F
	  1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0, //90-9F
	  0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1, //A0-AF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //B0-BF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //C0-CF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //D0-DF
	  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, //E0-EF
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 //F0-FF
	}
};

BOOL CharHandleV( HIMC hIMC,WORD wParam,LONG lParam)
{
    LPINPUTCONTEXT lpIMC;
	LPCANDIDATEINFO lpCandInfo;
	LPCANDIDATELIST lpCandList;
	LPCOMPOSITIONSTRING lpCompStr;
	GENEMSG GnMsg;
	WORD wCount;
	WORD wTemp;
	static BYTE bHead,bDirect;
	static WORD wPos,wPrePos;
	BYTE bPunct[4];

#define FORWARD 0x01
#define BACKWARD 0x02

	bPunct[2] = 0;
	bPunct[3] = 0;

    lpIMC = ImmLockIMC(hIMC);
	lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
	lpCandList = (LPCANDIDATELIST)((LPSTR)lpCandInfo  + lpCandInfo->dwOffset[0]);
	lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);

	if( !lpCandList->dwCount ){
		if( wParam >= _T('1') && wParam <= _T('9') ) {
			LPTSTR lpStr;
			WORD wStrLen;

			wPos = 0x40;
			wPrePos = 0x40;
			bDirect = FORWARD;
			bHead =(BYTE)( wParam - _T('1'));
			lpCandList->dwPageSize = 10;
			lpCandList->dwPageStart = 2;

			wCount = 0;
			while( wPos <= 0xFF ) {
				if(bPunctChar[bHead][wPos-0x40]){
					bPunct[0] = 0xA1 + bHead;
					bPunct[1] = (BYTE)wPos;
					memcpy((LPBYTE)GETLPCANDSTR(lpCandList,wCount+2),bPunct,4);
					wCount++;
				}
				wPos++;
				if(wCount == lpCandList->dwPageSize) break;
			}
			lpCandList->dwCount = wCount;

			lpStr = GETLPCOMPSTR(lpCompStr);
			wStrLen = _tcslen(lpStr);
			*(lpStr + wStrLen) = (TCHAR)wParam;
			*(lpStr + wStrLen +1) = _T('\0');

			lpStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPaintCompStr;
			wStrLen = _tcslen(lpStr);
			*(lpStr + wStrLen) = (TCHAR)wParam;
			*(lpStr + wStrLen +1) = _T('\0');

			GnMsg.msg = WM_IME_COMPOSITION;
			GnMsg.wParam = 0;
			GnMsg.lParam = GCS_COMPSTR;
			GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
		}
		else MessageBeep(0xFFFFFFFF );
	}
	else{
		if( wParam == _T('=') || wParam == _T('.') || wParam == _T('>')) {
			if( bDirect & BACKWARD ) {
				bDirect = FORWARD;
				wPos = wPrePos+1;
			}
			DebugLog(4,(DebugLogFile,"---forward: %lx %lx\n",wPrePos,wPos));
			if (wPos >= 0xFF ) {
				MessageBeep(0xFFFFFFFF );
				goto my_exit;
			}
			if(wPos < 0x40 ) wPos = 0x40;

			wPrePos = wPos;
			wCount = 0;
			while( wPos <= 0xFF ) {
				if(bPunctChar[bHead][wPos-0x40]){
					bPunct[0] = 0xA1 + bHead;
					bPunct[1] = (BYTE)wPos;
					memcpy((LPBYTE)GETLPCANDSTR(lpCandList,wCount+2),bPunct,4);
					wCount++;
				}
				wPos++;
				if(wCount == lpCandList->dwPageSize) break;
			}
			lpCandList->dwCount = wCount;
			DebugLog(4,(DebugLogFile,"forward: %lx %lx\n",wPrePos,wPos));
			GnMsg.msg = WM_IME_COMPOSITION;
			GnMsg.wParam = 0;
			GnMsg.lParam = GCS_COMPSTR;
			GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
		}
		else if( wParam == _T('-') || wParam == _T(',') || wParam == _T('<')) {
			TCHAR szStrArr[30][4];

			DebugLog(4,(DebugLogFile,"+++backward: %lx %lx\n",wPrePos,wPos));
			if( bDirect & FORWARD ) {
				bDirect = BACKWARD;
				wPos = wPrePos-1;
			}
			DebugLog(4,(DebugLogFile,"---backward: %lx %lx\n",wPrePos,wPos));
			if (wPos < 0x40 ) {
				MessageBeep(0xFFFFFFFF );
				goto my_exit;
			}
			if(wPos > 0xFF) wPos = 0xFF;

			wPrePos = wPos;
			wCount = 0;
			while( wPos >= 0x40 ) {
				if(bPunctChar[bHead][wPos-0x40]){
					bPunct[0] = 0xA1 + bHead;
					bPunct[1] = (BYTE)wPos;
					memcpy((LPBYTE)szStrArr[wCount],bPunct,4);
					wCount++;
				}
				wPos--;
				if(wCount == lpCandList->dwPageSize) break;
			}
			DebugLog(4,(DebugLogFile,"backward: %lx %lx\n",wPrePos,wPos));
			if( wCount == 0){
				MessageBeep(0xFFFFFFFF );
				goto my_exit;
			}
			lpCandList->dwCount = wCount;
			wTemp = 2;
			while(wCount--){
				_tcscpy(GETLPCANDSTR(lpCandList,wTemp++),szStrArr[wCount]);
			}

			GnMsg.msg = WM_IME_COMPOSITION;
			GnMsg.wParam = 0;
			GnMsg.lParam = GCS_COMPSTR;
			GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
		}
		else if( wParam >= _T('0') && wParam <= _T('9') ){
			SelectCandFromCandlist(hIMC, wParam);
		}
	}
my_exit:
	ImmUnlockIMCC(lpIMC->hCompStr);
	ImmUnlockIMCC(lpIMC->hCandInfo);
	ImmUnlockIMC(hIMC);

#undef FORWARD
#undef BACKWARD

	return TRUE;
}
