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

BOOL WINAPI ImeInquire(LPIMEINFO lpIMEInfo,LPTSTR lpszUIClass,LPCTSTR lpszOption)
{
	DebugLog(1,(DebugLogFile,"ImeInquire\n"));

    lpIMEInfo->dwPrivateDataSize = sizeof(UIEXTRA);

    lpIMEInfo->fdwProperty = IME_PROP_KBD_CHAR_FIRST |
#ifdef _UNICODE
                             IME_PROP_UNICODE |
#endif
                             IME_PROP_SPECIAL_UI;

    lpIMEInfo->fdwConversionCaps = IME_CMODE_FULLSHAPE |
								IME_CMODE_NATIVE;

    lpIMEInfo->fdwSentenceCaps = IME_SMODE_NONE;
    lpIMEInfo->fdwUICaps = UI_CAP_2700;

	lpIMEInfo->fdwSCSCaps = 0;

    lpIMEInfo->fdwSelectCaps = SELECT_CAP_CONVERSION;

    _tcscpy(lpszUIClass,UICLASSNAME);

    return TRUE;
}

BOOL WINAPI ImeConfigure(HKL hKL,HWND hWnd, DWORD dwMode, LPVOID lpData)
{
	DebugLog(1,(DebugLogFile,"ImeConfigure\n"));
	DialogBox( hInst,MAKEINTRESOURCE(DIALOGCONFIG),hWnd,ConfigDialogProc);
	InvalidateRect(hWnd,NULL,FALSE);
	return TRUE;
}

DWORD WINAPI ImeConversionList(HIMC hIMC,LPCTSTR lpSource,LPCANDIDATELIST lpCandList,DWORD dwBufLen,UINT uFlag)
{
	DebugLog(1,(DebugLogFile,"ImeConversionList\n"));
    return 0;
}

BOOL WINAPI ImeDestroy(UINT uForce)
{
	DebugLog(1,(DebugLogFile,"ImeDestroy\n"));
    return FALSE;
}

LRESULT WINAPI ImeEscape(HIMC hIMC,UINT uSubFunc,LPVOID lpData)
{
	DebugLog(1,(DebugLogFile,"ImeEscape\n"));
	return FALSE;
}

BOOL WINAPI ImeProcessKey(HIMC hIMC,UINT vKey,LPARAM lKeyData,CONST LPBYTE lpbKeyState)
{
    BOOL fRet = FALSE;
    BOOL fOpen;
    BOOL fCompStr = FALSE;
    LPINPUTCONTEXT lpIMC;
    LPCOMPOSITIONSTRING lpCompStr;
	static BOOL fPressOther = FALSE;
//	static BOOL fFirst = TRUE;

	DebugLog(1,(DebugLogFile,"ImeProcessKey\n"));

    if ((lKeyData & 0x80000000) && vKey != VK_CONTROL)
        return FALSE;

	if (!(lKeyData & 0x80000000) && vKey == VK_CONTROL)
		return FALSE;

	if (lpbKeyState[VK_MENU] & 0x80 ) return FALSE;

	if(vKey != VK_CONTROL && lpbKeyState[VK_CONTROL] & 0x80 ) {
		fPressOther = TRUE;
		return FALSE;
	}

    if (!(lpIMC = ImmLockIMC(hIMC)))
        return FALSE;

    fOpen = lpIMC->fOpen;

	if(vKey == VK_CONTROL && (lKeyData & 0x80000000) && !fPressOther){ // && !fFirst){
		GENEMSG GnMsg;

		fPressOther = FALSE;

		if(fOpen) {
			lpIMC->fOpen=FALSE;
			MakeResultString(hIMC,FALSE);
		}
		else lpIMC->fOpen=TRUE;

		GnMsg.msg = WM_IME_NOTIFY;
		GnMsg.wParam = IMN_SETOPENSTATUS;
		GnMsg.lParam = 0;
		GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
		return FALSE;
	}

	fPressOther = FALSE;

//	if(fFirst) fFirst = FALSE;

/*
	//if CapsLock is pressed ,then return false;
	if (lpbKeyState[VK_CAPITAL] & 0x01) {
		return FALSE;
	}

    if (fOpen)
    {
        if (lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr))
        {
            if ((lpCompStr->dwSize > sizeof(COMPOSITIONSTRING)) && 
                (lpCompStr->dwCompStrLen))
                fCompStr = TRUE;
			ImmUnlockIMCC(lpIMC->hCompStr);
        }

		if (dwConversionMode & CONVERSION_MODE_FULLSHAPE){
			if (lpbKeyState[VK_SHIFT] & 0x80)
		    {
			    if (fCompStr)
				    fRet = (BOOL)bFullCompSht[vKey];
				else
					fRet = (BOOL)bFullNoCompSht[vKey];
			}
			else 
			{
				if (fCompStr)
					fRet = (BOOL)bFullComp[vKey];
				else
					fRet = (BOOL)bFullNoComp[vKey];
			}
		}
		else{
			if (lpbKeyState[VK_SHIFT] & 0x80)
		    {
			    if (fCompStr)
				    fRet = (BOOL)bCompSht[vKey];
				else
					fRet = (BOOL)bNoCompSht[vKey];
			}
			else 
			{
				if (fCompStr)
					fRet = (BOOL)bComp[vKey];
				else
					fRet = (BOOL)bNoComp[vKey];
			}
		}
    }
*/

    if (fOpen)
    {
        if (lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr))
        {
            if ((lpCompStr->dwSize > sizeof(COMPOSITIONSTRING)) && 
                (lpCompStr->dwCompStrLen))
                fCompStr = TRUE;
			ImmUnlockIMCC(lpIMC->hCompStr);
        }
		if (fCompStr)
			fRet = (BOOL)bComp[vKey];
		else
			fRet = (BOOL)bNoComp[vKey];
	}

    ImmUnlockIMC(hIMC);
    return fRet;
}


BOOL WINAPI ImeSelect(HIMC hIMC, BOOL fSelect)
{
    LPINPUTCONTEXT lpIMC;

	DebugLog(1,(DebugLogFile,"ImeSelect\n"));

    if (fSelect)
        UpdateIndicIcon(hIMC);

    // it's NULL context.
    if (!hIMC)
        return TRUE;

    if (lpIMC = ImmLockIMC(hIMC))
    {
        if (fSelect)
        {
            LPCOMPOSITIONSTRING lpCompStr;
            LPCANDIDATEINFO lpCandInfo;
			HKEY hKey;
			DWORD dwConvSet;
			DWORD dwData = sizeof(DWORD);
			BOOL fFlag = TRUE;

            // Init the general member of IMC.
			lpIMC->fOpen=TRUE;
			
            lpIMC->hCompStr = ImmReSizeIMCC(lpIMC->hCompStr,sizeof(MYCOMPSTR));
            if (lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr)) 
            {
				InitCompStr(lpCompStr);
                ImmUnlockIMCC(lpIMC->hCompStr);
            }
            lpIMC->hCandInfo = ImmReSizeIMCC(lpIMC->hCandInfo,sizeof(MYCAND));
            if (lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo)) 
            {
				InitCandInfo(lpCandInfo);
                ImmUnlockIMCC(lpIMC->hCandInfo);
            }

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
					wConversionSet = (WORD) dwConvSet;
					fFlag = FALSE;
				}
				RegCloseKey(hKey);
			}
			if( fFlag) {
				wConversionSet = CONVERSION_SET_PUNCT;
				wConversionSet |= CONVERSION_SET_SORT;
				wConversionSet |= CONVERSION_SET_FOLLOW;
				wConversionSet |= CONVERSION_SET_GBK;
			}

        }
    }

    ImmUnlockIMC(hIMC);
	
    return TRUE;
}

BOOL WINAPI ImeSetActiveContext(HIMC hIMC,BOOL fFlag)
{
	DebugLog(1,(DebugLogFile,"ImeSetActiveContext\n"));

    UpdateIndicIcon(hIMC);

    return TRUE;
}

UINT WINAPI ImeToAsciiEx (UINT uVKey,UINT uScanCode,CONST LPBYTE lpbKeyState,LPDWORD lpdwTransKey,UINT fuState,HIMC hIMC)
{
    LPARAM lParam;
    LPINPUTCONTEXT lpIMC;
    BOOL fOpen;

	DebugLog(1,(DebugLogFile,"ImeToAsciiEx\n"));

    lpdwCurTransKey = lpdwTransKey;
    lParam = ((DWORD)uScanCode << 16) + 1L;
    
    // Init uNumTransKey here.
    uNumTransKey = 0;

    // if hIMC is NULL, this means DISABLE IME.
    if (!hIMC)
        return 0;
    if (!(lpIMC = ImmLockIMC(hIMC)))
        return 0;
    fOpen = lpIMC->fOpen;
    ImmUnlockIMC(hIMC);

    // The current status of IME is "closed".
    if (!fOpen)
        goto my_exit;

    if (uScanCode & 0x8000)
        IMEKeyupHandler( hIMC, uVKey, lParam, lpbKeyState);
    else
        IMEKeydownHandler( hIMC, uVKey, lParam, lpbKeyState);


    // Clear static value, no more generated message!
    lpdwCurTransKey = NULL;

my_exit:

    // If trans key buffer that is allocated by USER.EXE full up,
    // the return value is the negative number.
    if (fOverTransKey)
    {
        return (int)uNumTransKey;
    }

    return (int)uNumTransKey;
}

BOOL WINAPI NotifyIME(HIMC hIMC,DWORD dwAction,DWORD dwIndex,DWORD dwValue)
{
    BOOL bRet = FALSE;
	LPINPUTCONTEXT lpIMC;

	DebugLog(1,(DebugLogFile,"NotifyIME\n"));
	
    switch(dwAction)
    {
	case NI_OPENCANDIDATE:
		DebugLog(1,(DebugLogFile,"NotifyIME:NI_OPENCANDIDATE\n"));
		break;
	case NI_CLOSECANDIDATE:
		DebugLog(1,(DebugLogFile,"NotifyIME:NI_CLOSECANDIDATE\n"));
		break;
	case NI_SELECTCANDIDATESTR:
		DebugLog(1,(DebugLogFile,"NotifyIME:NI_SELECTCANDIDATESTR\n"));
		break;
	case NI_CHANGECANDIDATELIST:
		DebugLog(1,(DebugLogFile,"NotifyIME:NI_CHANGECANDIDATELIST\n"));
		break;
	case NI_SETCANDIDATE_PAGESTART:
		DebugLog(1,(DebugLogFile,"NotifyIME:NI_SETCANDIDATE_PAGESTART\n"));
		break;
	case NI_SETCANDIDATE_PAGESIZE:
		DebugLog(1,(DebugLogFile,"NotifyIME:NI_SETCANDIDATE_PAGESIZE\n"));
		break;
	case NI_CONTEXTUPDATED:
		DebugLog(1,(DebugLogFile,"NotifyIME:NI_CONTEXTUPDATED\n"));
		switch (dwValue)
		{
		case IMC_SETCONVERSIONMODE:
			DebugLog(1,(DebugLogFile,"NotifyIME:NI_CONTEXTUPDATED:IMC_SETCONVERSIONMODE\n"));
			break;
		case IMC_SETSENTENCEMODE:
			DebugLog(1,(DebugLogFile,"NotifyIME:NI_CONTEXTUPDATED:IMC_SETSENTENCEMODE\n"));
			break;
		case IMC_SETCANDIDATEPOS:
			DebugLog(1,(DebugLogFile,"NotifyIME:NI_CONTEXTUPDATED:IMC_SETCANDIDATEPOS\n"));
			break;
		case IMC_SETCOMPOSITIONFONT:
			DebugLog(1,(DebugLogFile,"NotifyIME:NI_CONTEXTUPDATED:IMC_SETCOMPOSITIONFONT\n"));
			break;
		case IMC_SETCOMPOSITIONWINDOW:
			DebugLog(1,(DebugLogFile,"NotifyIME:NI_CONTEXTUPDATED:IMC_SETCOMPOSITIONWINDOW\n"));
			break;
		case IMC_SETOPENSTATUS:
			DebugLog(1,(DebugLogFile,"NotifyIME:NI_CONTEXTUPDATED:IMC_SETOPENSTATUS\n"));
			lpIMC = ImmLockIMC(hIMC);
			if (lpIMC)
			{
				if (!lpIMC->fOpen && IsCompStr(hIMC))
					MakeResultString( hIMC,FALSE);
				ImmUnlockIMC(hIMC);
			}

			UpdateIndicIcon(hIMC);
			bRet = TRUE;
			break;
		default:
			break;
		}
		break;
		
	case NI_COMPOSITIONSTR:
		DebugLog(1,(DebugLogFile,"NotifyIME:NI_COMPOSITIONSTR\n"));
		switch (dwIndex)
		{
		case CPS_COMPLETE:
			DebugLog(1,(DebugLogFile,"NotifyIME:NI_COMPOSITIONSTR:CPS_COMPLETE\n"));
			break;

		case CPS_CONVERT:
			DebugLog(1,(DebugLogFile,"NotifyIME:NI_COMPOSITIONSTR:CPS_CONVERT\n"));
			break;
				
		case CPS_REVERT:
			DebugLog(1,(DebugLogFile,"NotifyIME:NI_COMPOSITIONSTR:CPS_REVERT\n"));
			break;
				
		case CPS_CANCEL:
			DebugLog(1,(DebugLogFile,"NotifyIME:NI_COMPOSITIONSTR:CPS_CANCEL\n"));
			break;
				
		default:
			break;
		}
		break;
			
	default:
		break;
    }
    return bRet;
}

BOOL WINAPI ImeRegisterWord(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr)
{
	DebugLog(1,(DebugLogFile,"ImeRegisterWord\n"));
    return FALSE;
}

BOOL WINAPI ImeUnregisterWord(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr)
{
	DebugLog(1,(DebugLogFile,"ImeUnregisterWord\n"));
    return FALSE;
}

UINT WINAPI ImeGetRegisterWordStyle(UINT nItem, LPSTYLEBUF lp)
{
	DebugLog(1,(DebugLogFile,"ImeGetRegisterWordStyle\n"));
	return 0;
}

UINT WINAPI ImeEnumRegisterWord(REGISTERWORDENUMPROC lpfn, LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr, LPVOID lpData)
{
	DebugLog(1,(DebugLogFile,"ImeEnumRegisterWord\n"));
	return 0;
}

BOOL WINAPI ImeSetCompositionString(HIMC hIMC, DWORD dwIndex, LPCVOID lpComp, DWORD dwComp, LPCVOID lpRead, DWORD dwRead)
{
	DebugLog(1,(DebugLogFile,"ImeSetCompositionString\n"));
    return FALSE;
}
