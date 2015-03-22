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

/**********************************************************************/
/*                                                                    */
/* IMEKeyupHandler()                                                  */
/*                                                                    */
/* A function which handles WM_IMEKEYUP                               */
/*                                                                    */
/**********************************************************************/
BOOL IMEKeyupHandler(
	HIMC hIMC,
	WPARAM wParam,
	LPARAM lParam,
	LPBYTE lpbKeyState)
{
    return FALSE;
}

/**********************************************************************/
/*                                                                    */
/* IMEKeydownHandler()                                                */
/*                                                                    */
/* A function which handles WM_IMEKEYDOWN                             */
/*                                                                    */
/**********************************************************************/
BOOL IMEKeydownHandler(
	HIMC hIMC,
	WPARAM wParam,
	LPARAM lParam,
	LPBYTE lpbKeyState)
{
    WORD wVKey;


    if( wVKey = ( wParam & 0x00FF ) ){
		if( !KeydownHandler( hIMC, wVKey, lParam, lpbKeyState ) ) {
		// This WM_IMEKEYDOWN has actual character code in itself.
#ifdef _UNICODE
			CharHandler( hIMC,  HIWORD(wParam), lParam );
#else
			CharHandler( hIMC,(WORD)((BYTE)HIBYTE(wParam)),lParam );
#endif
		}
    }
    return TRUE;
}


/**********************************************************************/
/*                                                                    */
/* KeydownHandler()                                                */
/*                                                                    */
/* WM_KEYDOWN handler for dictionary routine                          */
/*                                                                    */
/* wParam                                                             */
/* virtual key                                                        */
/*                                                                    */
/* lParam                                                             */
/* differ depending on wParam                                         */
/*                                                                    */
/**********************************************************************/
BOOL KeydownHandler(
	HIMC hIMC,
	WORD wParam,
	LONG lParam,
	LPBYTE lpbKeyState)
{
	LPINPUTCONTEXT lpIMC;
	LPCOMPOSITIONSTRING lpCompStr;
	GENEMSG GnMsg;
	SHORT *lpwEditCaret;
	SHORT wUnConvPos;
	LPTSTR lpStr;

	lpIMC = ImmLockIMC(hIMC);
	lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
	lpwEditCaret = &(((LPMYCOMPSTR)lpCompStr)->FreePYComp.wEditCaret);
	wUnConvPos = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.wUnConvPos;
	lpStr = GETLPCOMPSTR(lpCompStr);

    switch( wParam ) 
    {
        case VK_ESCAPE:
            MakeResultString(hIMC,FALSE); //clear all char
            break;
		case VK_RETURN: //to PhraseToChar mode 
			{
				LPCANDIDATEINFO lpCandInfo;
				LPCANDIDATELIST lpCandList;
				LPTSTR lpPaintStr;

				lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
				lpCandList = (LPCANDIDATELIST)((LPSTR)lpCandInfo  + lpCandInfo->dwOffset[0]);

				if( lpCandList->dwCount ){
					wConversionMode |= CONVERSION_MODE_PHRASETOCHAR;
				
					lpPaintStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPaintCompStr;
					_tcscpy(lpPaintStr,PHTOCHMODEL);

					GnMsg.msg = WM_IME_COMPOSITION;
					GnMsg.wParam = 0;
					GnMsg.lParam = GCS_COMPSTR;
					GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
				}
				ImmUnlockIMCC(lpIMC->hCandInfo);
				break;
			}

        case VK_BACK:
            DeleteCharBackward(hIMC,wParam); //delete char
            break;

        case VK_DELETE:
			DeleteCharForward(hIMC,wParam);
			break;

		case VK_HOME:
			if( *lpwEditCaret > 0 ) {
				if( wUnConvPos ){
					*lpwEditCaret = wUnConvPos;
				}
				else {
					*lpwEditCaret = 0;
				}
				GnMsg.msg = WM_IME_NOTIFY;
				GnMsg.wParam = IMN_SETCOMPOSITIONWINDOW;
				GnMsg.lParam = 0;
				GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
			}
			else MessageBeep(0xFFFFFFFF );

			break;

		case VK_END:
			if( *lpwEditCaret < (SHORT)_tcslen(lpStr) ) {
				*lpwEditCaret = (SHORT)_tcslen(lpStr);
				GnMsg.msg = WM_IME_NOTIFY;
				GnMsg.wParam = IMN_SETCOMPOSITIONWINDOW;
				GnMsg.lParam = 0;
				GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
			}
			else MessageBeep(0xFFFFFFFF );

			break;

		case VK_LEFT:
		case VK_UP:
			if( *lpwEditCaret > 0 ) {
				if( *lpwEditCaret == wUnConvPos) AddChar(hIMC,wParam,EDIT_BACK);
				else {
					*lpwEditCaret -= 1;
					GnMsg.msg = WM_IME_NOTIFY;
					GnMsg.wParam = IMN_SETCOMPOSITIONWINDOW;
					GnMsg.lParam = 0;
					GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
				}
			}
			else MessageBeep(0xFFFFFFFF );
			break;

		case VK_RIGHT:
		case VK_DOWN:
			if( *lpwEditCaret < (SHORT)_tcslen(lpStr) ) {
				*lpwEditCaret += 1;
				GnMsg.msg = WM_IME_NOTIFY;
				GnMsg.wParam = IMN_SETCOMPOSITIONWINDOW;
				GnMsg.lParam = 0;
				GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
			}
			else MessageBeep(0xFFFFFFFF );
			break;

        case VK_SPACE:
			SpaceSelect(hIMC,wParam);
            break;
        default:
            break;
    }

	ImmUnlockIMCC(lpIMC->hCompStr);
	ImmUnlockIMC(hIMC);

    if (( VK_0 <= wParam && VK_9 >= wParam ) ||
        ( VK_A <= wParam && VK_Z >= wParam ) ||
        ( VK_NUMPAD0 <= wParam && VK_NUMPAD9 >= wParam ) ||
        ( VK_OEM_1 <= wParam && VK_OEM_9 >= wParam ) ||
        ( VK_MULTIPLY <= wParam && VK_DIVIDE >= wParam ))
    {
        return( FALSE );
    }
    else
        return( TRUE );
}

BOOL CharHandler(
	HIMC hIMC,
	WORD wParam,
	LONG lParam)
{
    LPINPUTCONTEXT lpIMC;
    LPCOMPOSITIONSTRING lpCompStr;
	LPCANDIDATEINFO lpCandInfo;
    DWORD dwStrLen;
    GENEMSG GnMsg;
	LPTSTR lpConvStr;

    lpIMC = ImmLockIMC(hIMC);


	lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
    dwStrLen = lpCompStr->dwCompStrLen;

    if (!dwStrLen)
    {
		lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
		ClearCompStr(lpCompStr);
		ClearCandidate(lpCandInfo);
		ImmUnlockIMCC(lpIMC->hCandInfo);

        GnMsg.msg = WM_IME_STARTCOMPOSITION;
        GnMsg.wParam = 0;
        GnMsg.lParam = 0;
        GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);

		if(wParam == _T('#') ) {
			ImmUnlockIMCC(lpIMC->hCompStr);
		    ImmUnlockIMC(hIMC);
			return RepeatPreResult(hIMC);
		}

		if( (wParam < _T('a') || wParam > _T('z')) &&
			(wParam < _T('A') || wParam > _T('Z')) &&
			(wParam < _T('0') || wParam > _T('9')) ){
			ImmUnlockIMCC(lpIMC->hCompStr);
		    ImmUnlockIMC(hIMC);
			return GeneratePunct(hIMC,wParam);
		}

		if( wParam == _T('i') ) {
			LPTSTR lpStr;

			wConversionMode |= CONVERSION_MODE_I;

			lpStr = GETLPCOMPSTR(lpCompStr);
			*lpStr = (TCHAR)wParam;
			*(lpStr+1) = _T('\0');
			lpCompStr->dwCompStrLen = _tcslen(lpStr);

			lpStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPaintCompStr;
			_tcscpy(lpStr,IMODEL);

			GnMsg.msg = WM_IME_COMPOSITION;
			GnMsg.wParam = 0;
			GnMsg.lParam = GCS_COMPSTR;
			GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
			goto my_exit;
		}
		if( wParam == _T('u') ) {
			LPTSTR lpStr;

			wConversionMode |= CONVERSION_MODE_U;

			lpStr = GETLPCOMPSTR(lpCompStr);
			*lpStr = (TCHAR)wParam;
			*(lpStr+1) = _T('\0');
			lpCompStr->dwCompStrLen = _tcslen(lpStr);

			lpStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPaintCompStr;
			_tcscpy(lpStr,UMODEL);

			GnMsg.msg = WM_IME_COMPOSITION;
			GnMsg.wParam = 0;
			GnMsg.lParam = GCS_COMPSTR;
			GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
			goto my_exit;
		}
		if( wParam == _T('v') ) {
			LPTSTR lpStr;

			wConversionMode |= CONVERSION_MODE_V;

			lpStr = GETLPCOMPSTR(lpCompStr);
			*lpStr = (TCHAR)wParam;
			*(lpStr+1) = _T('\0');
			lpCompStr->dwCompStrLen = _tcslen(lpStr);

			lpStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPaintCompStr;
			_tcscpy(lpStr,VMODEL);

			GnMsg.msg = WM_IME_COMPOSITION;
			GnMsg.wParam = 0;
			GnMsg.lParam = GCS_COMPSTR;
			GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
			goto my_exit;
		}
    }

	if(wConversionMode & CONVERSION_MODE_PHRASETOCHAR) {
		if( CharHandlePhToCh(hIMC,wParam,lParam) ) goto my_exit;
	}
	else if(wConversionMode & CONVERSION_MODE_I) {
		if( CharHandleI(hIMC,wParam,lParam) ) goto my_exit;
	}
	else if(wConversionMode & CONVERSION_MODE_U) {
		if( CharHandleU(hIMC,wParam,lParam) ) goto my_exit;
	}
	else if(wConversionMode & CONVERSION_MODE_V) {
		if( CharHandleV(hIMC,wParam,lParam) ) goto my_exit;
	}
	else{
		if( CharHandleNormal(hIMC,wParam,lParam) ) goto my_exit;
	}

	if( wParam >= _T('!') && wParam <= _T('~') && lpCompStr->dwCompStrLen == 0) {
		lpConvStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szConvCompStr;
		if(wConversionSet & CONVERSION_SET_FULLSHAPE) {
			BYTE bChar[4];
			bChar[0] = 0xA3;
			bChar[1] = (BYTE)wParam + 0x80;
			bChar[2] = 0;
			bChar[3] = 0;
			memcpy((LPBYTE)lpConvStr,bChar,4);
		}
		else{
			*lpConvStr = (TCHAR)wParam;
			*(lpConvStr+1) = _T('\0');
		}
		MakeResultString(hIMC,TRUE);
	}

my_exit:
	ImmUnlockIMCC(lpIMC->hCompStr);
    ImmUnlockIMC(hIMC);
	return TRUE;
}
