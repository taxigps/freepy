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

BOOL CharHandleNormal( HIMC hIMC,WORD wParam,LONG lParam)
{
	if( (wParam >= _T('a') && wParam <= _T('z')) || wParam == _T('\'') ) {
		return AddChar(hIMC,wParam,EDIT_ADD);
	}
	else if( wParam == _T('=') || wParam == _T('.') || wParam == _T('>')) {
		return ForwardPage(hIMC);
	}

	else if( wParam == _T('-') || wParam == _T(',') || wParam == _T('<')) {
		return BackwardPage(hIMC);
	}
	else if( wParam >= _T('0') && wParam <= _T('9') ){
		return SelectCand(hIMC,wParam);
	}
	switch ( wParam ){
	case _T('!'):
	case _T('@'):
	case _T('#'):
	case _T('$'):
	case _T('%'):
	case _T('^'):
	case _T('&'):
	case _T('*'):
	case _T('('):
	case _T(')'):
		return DeletePhrase(hIMC,wParam);
	default:
		break;
	}
	return FALSE;
}

BOOL AddChar( HIMC hIMC, WORD wCode, WORD wEditMode)
{
    LPINPUTCONTEXT lpIMC;
    LPCOMPOSITIONSTRING lpCompStr;
	LPCANDIDATEINFO lpCandInfo;
	LPCANDIDATELIST lpCandList;
	LPFREEPYCAND lpPYCand;

	LPTSTR lpStr;
	LPTSTR lpConvStr;
	LPTSTR lpPaintStr;
	TCHAR  szTemp[150];
    GENEMSG GnMsg;
	LPTSTR lpPYArray;
	WORD *lpwPYArrayLen;
	WORD *lpwPYArrayCurPos;
	SHORT *lpwUnConvPos;
	SHORT *lpwEditCaret;
	TCHAR cTempChar;
	WORD wPYCount;
	int i;
	BOOL fRet = FALSE;
	TCHAR szPreStr[150];
	WORD  wPreUnConvPos;
	WORD  wPreEditCaret;

    lpIMC = ImmLockIMC(hIMC);

	lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
	lpStr = GETLPCOMPSTR(lpCompStr);
	lpConvStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szConvCompStr;
	lpPaintStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPaintCompStr;
	lpPYArray = (LPTSTR)(((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPYArray);
	lpwPYArrayLen = &(((LPMYCOMPSTR)lpCompStr)->FreePYComp.wPYArrayLen);
	lpwPYArrayCurPos = &(((LPMYCOMPSTR)lpCompStr)->FreePYComp.wPYArrayCurPos);
	lpwUnConvPos = &(((LPMYCOMPSTR)lpCompStr)->FreePYComp.wUnConvPos);
	lpwEditCaret = &(((LPMYCOMPSTR)lpCompStr)->FreePYComp.wEditCaret);

	_tcscpy(szPreStr,lpStr);
	wPreUnConvPos = *lpwUnConvPos;
	wPreEditCaret = *lpwEditCaret;

    if( wEditMode == EDIT_BACK ){
		if(!_tcslen(lpStr) || (*lpwEditCaret) < 1 ){
			MessageBeep(0xFFFFFFFF );
			fRet = TRUE;
			goto my_exit;
		}
		else if(*lpwEditCaret <= *lpwUnConvPos){
			*lpConvStr = _T('\0');
			*lpwUnConvPos = 0;
		}
		else{
			_tcscpy(szTemp,lpStr + *lpwEditCaret);
			_tcscpy(lpStr + *lpwEditCaret -1,szTemp);
			*lpwEditCaret -= 1;
			if( !_tcslen(lpStr)){
				MakeResultString(hIMC,FALSE);
				fRet = TRUE;
				goto my_exit;
			}
		}
    }
    else if( wEditMode == EDIT_DELETE ){
		if(!_tcslen(lpStr) || (*lpwEditCaret) == (SHORT)_tcslen(lpStr) ){
			MessageBeep(0xFFFFFFFF );
			fRet = TRUE;
			goto my_exit;
		}
		else{
			_tcscpy(szTemp,lpStr + *lpwEditCaret + 1);
			_tcscpy(lpStr + *lpwEditCaret,szTemp);
			if( !_tcslen(lpStr)){
				MakeResultString(hIMC,FALSE);
				fRet = TRUE;
				goto my_exit;
			}
		}
    }
    else if( wEditMode == EDIT_ADD ){
		_tcscpy(szTemp,lpStr + *lpwEditCaret);
		*(lpStr + *lpwEditCaret) = (TCHAR)wCode;
		_tcscpy(lpStr + *lpwEditCaret +1,szTemp);
		*lpwEditCaret += 1;
    }
	else{
		MessageBeep(0xFFFFFFFF );
		fRet = TRUE;
		goto my_exit;
	}

    lpCompStr->dwCompStrLen = _tcslen(lpStr);


    if(!_tcslen(lpConvStr)){
		*lpwPYArrayCurPos = 0;
    }
	
    wPYCount=ParsePY(lpStr + *lpwUnConvPos,lpPYArray + (*lpwPYArrayCurPos)*MAX_PY_LEN,MAX_PY_LEN);
	
	*lpwPYArrayLen = *lpwPYArrayCurPos + wPYCount;
	i = *lpwPYArrayLen;
	while(i--){
		if((cTempChar = *(lpPYArray + i*MAX_PY_LEN)) == _T('i') || 
				cTempChar == _T('u') || cTempChar == _T('v') ){
			_tcscpy(lpStr,szPreStr);
			*lpwEditCaret = wPreEditCaret;
			*lpwUnConvPos = wPreUnConvPos;
			*lpwPYArrayLen = *lpwPYArrayLen - 1;
			MessageBeep(0xFFFFFFFF );
			fRet = TRUE;
			goto my_exit;
		}
	}
	
    if(EffectPYArrayLen(lpPYArray,MAX_PY_LEN,*lpwPYArrayLen) > MAX_PHRASE_LEN) {
		_tcscpy(lpStr,szPreStr);
		*lpwEditCaret = wPreEditCaret;
		*lpwUnConvPos = wPreUnConvPos;
		*lpwPYArrayLen = *lpwPYArrayLen - 1;
		MessageBeep(0xFFFFFFFF );
		fRet = TRUE;
		goto my_exit;
    }
	lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
	lpCandList = (LPCANDIDATELIST)((LPBYTE)lpCandInfo  + lpCandInfo->dwOffset[0]);
	lpPYCand = (LPFREEPYCAND)(&((LPMYCAND)lpCandInfo)->FreePYCand);

	ConvertPY( lpPYArray + (*lpwPYArrayCurPos)*MAX_PY_LEN,
				MAX_PY_LEN,wPYCount,lpPYCand);

    lpPYCand->awBeforePos[1] = 0;
	lpPYCand->awBeforePos[0] = 0;
	lpPYCand->awCurrentPos[1] = 0;
	lpPYCand->awCurrentPos[0] = 0;
	lpPYCand->wSelectDirect = SELECT_FORWARD;
    lpPYCand->wSelectStatus = 1;

	lpCandList->dwCount = CreateCandStr(lpPYCand, SELECT_FORWARD, (LPTSTR)(((LPMYCAND)lpCandInfo)->szCandStr),MAXCANDSTRSIZE);
	lpCandList->dwPageStart = 2;
	lpCandList->dwPageSize = 10;

	CreatePaintStr( lpPYArray + (*lpwPYArrayCurPos)*MAX_PY_LEN,
			MAX_PY_LEN,(WORD)((*lpwPYArrayLen)-(*lpwPYArrayCurPos)),lpConvStr,lpPaintStr);

    GnMsg.msg = WM_IME_COMPOSITION;
    GnMsg.wParam = 0;
    GnMsg.lParam = GCS_COMPSTR;
    GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);

	ImmUnlockIMCC(lpIMC->hCandInfo);
	fRet = TRUE;
my_exit:
    ImmUnlockIMCC(lpIMC->hCompStr);
    ImmUnlockIMC(hIMC);
	return fRet;
}

BOOL ForwardPage(HIMC hIMC)
{
    LPINPUTCONTEXT lpIMC;
	LPCANDIDATEINFO lpCandInfo;
	LPCANDIDATELIST lpCandList;
	LPFREEPYCAND lpPYCand;
	GENEMSG GnMsg;

	if( !IsCompStr(hIMC) ) return FALSE;

	lpIMC = ImmLockIMC(hIMC);
	lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
	lpCandList = (LPCANDIDATELIST)((LPBYTE)lpCandInfo  + lpCandInfo->dwOffset[0]);
	lpPYCand = (LPFREEPYCAND)(&((LPMYCAND)lpCandInfo)->FreePYCand);

	lpCandList->dwCount = 
		CreateCandStr(lpPYCand, SELECT_FORWARD, (LPTSTR)(((LPMYCAND)lpCandInfo)->szCandStr),MAXCANDSTRSIZE);
	lpCandList->dwPageStart = 2;
	lpCandList->dwPageSize = 10;

	if(lpCandList->dwCount) {
		GnMsg.msg = WM_IME_COMPOSITION;
		GnMsg.wParam = 0;
		GnMsg.lParam = GCS_COMPSTR;
		GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
	}
	ImmUnlockIMCC(lpIMC->hCandInfo);
    ImmUnlockIMC(hIMC);
	return TRUE;
}

BOOL BackwardPage(HIMC hIMC)
{
    LPINPUTCONTEXT lpIMC;
	LPCANDIDATEINFO lpCandInfo;
	LPCANDIDATELIST lpCandList;
	LPFREEPYCAND lpPYCand;
	GENEMSG GnMsg;

	if( !IsCompStr(hIMC) ) return FALSE;

	lpIMC = ImmLockIMC(hIMC);
	lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
	lpCandList = (LPCANDIDATELIST)((LPBYTE)lpCandInfo  + lpCandInfo->dwOffset[0]);
	lpPYCand = (LPFREEPYCAND)(&((LPMYCAND)lpCandInfo)->FreePYCand);

	lpCandList->dwCount = 
		CreateCandStr(lpPYCand, SELECT_BACKWARD, (LPTSTR)(((LPMYCAND)lpCandInfo)->szCandStr),MAXCANDSTRSIZE);
	lpCandList->dwPageStart = 2;
	lpCandList->dwPageSize = 10;

	if(lpCandList->dwCount) {
		GnMsg.msg = WM_IME_COMPOSITION;
		GnMsg.wParam = 0;
		GnMsg.lParam = GCS_COMPSTR;
		GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
	}

	ImmUnlockIMCC(lpIMC->hCandInfo);
    ImmUnlockIMC(hIMC);
	return TRUE;
}

BOOL SelectCand(HIMC hIMC,WORD wParam)
{
    LPINPUTCONTEXT lpIMC;
	LPCOMPOSITIONSTRING lpCompStr;
	LPCANDIDATEINFO lpCandInfo;
	LPCANDIDATELIST lpCandList;

	LPFREEPYCAND lpPYCand;

	LPTSTR lpStr;
	LPTSTR lpConvStr;
	LPTSTR lpPaintStr;
	LPTSTR lpPYArray;
	SHORT *lpwUnConvPos;
	SHORT *lpwEditCaret;
	WORD *lpwPYArrayLen;
	WORD *lpwPYArrayCurPos;
	GENEMSG GnMsg;
    WORD wIdx,awLen[2],wPosSpan,wTotal,wCount;
	TCHAR szTempStr[(MAX_PHRASE_LEN+1)*2];
	int i;

    if( !IsCompStr(hIMC) ) return FALSE;

	lpIMC = ImmLockIMC(hIMC);

	lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
	lpCandList = (LPCANDIDATELIST)((LPBYTE)lpCandInfo  + lpCandInfo->dwOffset[0]);
	lpPYCand = (LPFREEPYCAND)(&((LPMYCAND)lpCandInfo)->FreePYCand);

	lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
	lpStr = GETLPCOMPSTR(lpCompStr);
	lpConvStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szConvCompStr;
	lpPaintStr = ((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPaintCompStr;
	lpPYArray = (LPTSTR)(((LPMYCOMPSTR)lpCompStr)->FreePYComp.szPYArray);
	lpwPYArrayLen = &(((LPMYCOMPSTR)lpCompStr)->FreePYComp.wPYArrayLen);
	lpwPYArrayCurPos = &(((LPMYCOMPSTR)lpCompStr)->FreePYComp.wPYArrayCurPos);
	lpwUnConvPos = &(((LPMYCOMPSTR)lpCompStr)->FreePYComp.wUnConvPos);
	lpwEditCaret = &(((LPMYCOMPSTR)lpCompStr)->FreePYComp.wEditCaret);


    if( wParam == (WORD)_T('0') ) wIdx=9;
    else wIdx = wParam - (WORD)_T('1');

	if(wIdx >= lpCandList->dwCount) return FALSE;

	wTotal = wIdx + 1;
    switch(lpPYCand->wSelectDirect){
    case SELECT_FORWARD:
		if(wConversionSet & CONVERSION_SET_GBK) {
			wIdx += lpPYCand->awBeforePos[lpPYCand->wSelectStatus];
		}
		else {
			if(lpPYCand->wSelectStatus) {
				wIdx += lpPYCand->awBeforePos[lpPYCand->wSelectStatus];
			}
			else{
				wIdx = lpPYCand->awBeforePos[lpPYCand->wSelectStatus];
				wCount = 0;
				while( wCount != wTotal) {
					for(i=0;i < lpPYCand->wHZNum;i++){
						if((lpPYCand->aHanZi+i)->wLen > wIdx) break;
					}
#ifdef _UNICODE
					if(i)
						_tcsncpy(szTempStr,(lpPYCand->aHanZi+i)->lpHZ + (wIdx - (lpPYCand->aHanZi+i-1)->wLen),1);
					else
						_tcsncpy(szTempStr,(lpPYCand->aHanZi+i)->lpHZ + wIdx,1);
					szTempStr[1] = _T('\0');
#else
					if(i)
						_tcsncpy(szTempStr,(lpPYCand->aHanZi+i)->lpHZ + 2*(wIdx - (lpPYCand->aHanZi+i-1)->wLen),2);
					else
						_tcsncpy(szTempStr,(lpPYCand->aHanZi+i)->lpHZ+2*wIdx,2);
					szTempStr[2] = _T('\0');
#endif
					if( (*((LPBYTE)szTempStr) >= GB_QUFIRST && *((LPBYTE)szTempStr) <= GB_QULAST) && 
						(*((LPBYTE)szTempStr+1) >= GB_WEIFIRST && *((LPBYTE)szTempStr+1) <= GB_WEILAST) ) {
						wCount++;
					}
					wIdx++;
				}
				wIdx--;
			}
		}
		break;
    case SELECT_BACKWARD:
		if(wConversionSet & CONVERSION_SET_GBK) {
			wIdx += lpPYCand->awCurrentPos[lpPYCand->wSelectStatus]+1;
		}
		else {
			if(lpPYCand->wSelectStatus) {
				wIdx += lpPYCand->awCurrentPos[lpPYCand->wSelectStatus]+1;
			}
			else{
				wIdx = lpPYCand->awBeforePos[lpPYCand->wSelectStatus];
				wTotal--;
				wCount = 10;
				while( wCount != wTotal) {
					for(i=0;i < lpPYCand->wHZNum;i++){
						if((lpPYCand->aHanZi+i)->wLen > wIdx) break;
					}
#ifdef _UNICODE
					if(i)
						_tcsncpy(szTempStr,(lpPYCand->aHanZi+i)->lpHZ + (wIdx - (lpPYCand->aHanZi+i-1)->wLen),1);
					else
						_tcsncpy(szTempStr,(lpPYCand->aHanZi+i)->lpHZ + wIdx,1);
					szTempStr[1] = _T('\0');
#else
					if(i)
						_tcsncpy(szTempStr,(lpPYCand->aHanZi+i)->lpHZ + 2*(wIdx - (lpPYCand->aHanZi+i-1)->wLen),2);
					else
						_tcsncpy(szTempStr,(lpPYCand->aHanZi+i)->lpHZ+2*wIdx,2);
					szTempStr[2] = _T('\0');
#endif
					if( (*((LPBYTE)szTempStr) >= GB_QUFIRST && *((LPBYTE)szTempStr) <= GB_QULAST) && 
						(*((LPBYTE)szTempStr+1) >= GB_WEIFIRST && *((LPBYTE)szTempStr+1) <= GB_WEILAST) ) {
						wCount--;
					}
					wIdx--;
				}
				wIdx++;
			}
		}
		break;
    default:
		break;
    }
	awLen[1] = lpPYCand->wPhraseNum;
	awLen[0] = (lpPYCand->aHanZi + lpPYCand->wHZNum - 1)->wLen;

    if(0 < wIdx < awLen[lpPYCand->wSelectStatus]){
		if(lpPYCand->wSelectStatus){
			_tcscpy(szTempStr,(lpPYCand->aPhrase+wIdx)->lpHZPH->szHZ);
			_tcscat(lpConvStr,szTempStr);
			lpPYCand ->abKey[0] |= 
				(lpPYCand->aPhrase+wIdx)->lpKeyPH->abKey[0] << lpPYCand->wKeyLen;
			
			for(i=1;i<=(lpPYCand->aPhrase+wIdx)->lpKeyPH->wLen;i++){
				lpPYCand ->abKey[(lpPYCand->wKeyLen++) + 1] =
					(lpPYCand->aPhrase+wIdx)->lpKeyPH->abKey[i];
			}
			
		}
		else{
			for(i=0;i < lpPYCand->wHZNum;i++){
				if((lpPYCand->aHanZi+i)->wLen > wIdx ) break;
			}
#ifdef _UNICODE
			if(i)
				_tcsncpy(szTempStr,(lpPYCand->aHanZi+i)->lpHZ + (wIdx - (lpPYCand->aHanZi+i-1)->wLen),1);
			else
				_tcsncpy(szTempStr,(lpPYCand->aHanZi+i)->lpHZ + wIdx,1);
			szTempStr[1] = _T('\0');
#else
			if(i)
				_tcsncpy(szTempStr,(lpPYCand->aHanZi+i)->lpHZ + 2*(wIdx - (lpPYCand->aHanZi+i-1)->wLen),2);
			else
				_tcsncpy(szTempStr,(lpPYCand->aHanZi+i)->lpHZ+2*wIdx,2);
			szTempStr[2] = _T('\0');
#endif
			_tcscat(lpConvStr,szTempStr);
				
			lpPYCand ->abKey[0] |= ((lpPYCand->aHanZi+i)->wKey & 0x0100) >>(8 - lpPYCand->wKeyLen);
			lpPYCand ->abKey[(lpPYCand->wKeyLen++) + 1] = (BYTE)(lpPYCand->aHanZi+i)->wKey & 0xff;
				
		}
#ifdef _UNICODE
		if( EffectPYArrayLen(lpPYArray,MAX_PY_LEN,*lpwPYArrayLen) == _tcslen(lpConvStr) ) {
			if(_tcslen(szTempStr) == _tcslen(lpConvStr) && lpPYCand->wSelectStatus)
				(lpPYCand->aPhrase + wIdx)->lpHZPH->wAttrib++;
			else if(_tcslen(lpConvStr) > 1)
				SavePhToMapFile(lpConvStr,lpPYCand->abKey,(WORD)(_tcslen(lpConvStr)),1);
#else
		if( EffectPYArrayLen(lpPYArray,MAX_PY_LEN,*lpwPYArrayLen) == _tcslen(lpConvStr)/2 ) {
			if(_tcslen(szTempStr) == _tcslen(lpConvStr) && lpPYCand->wSelectStatus)
				(lpPYCand->aPhrase + wIdx)->lpHZPH->dwAttrib++;
			else if(_tcslen(lpConvStr) > 2)
				SavePhToMapFile(lpConvStr,lpPYCand->abKey,(WORD)(_tcslen(lpConvStr)/2),1);
#endif
			MakeResultString(hIMC,TRUE);
			goto my_exit;
		}
		else{
#ifdef _UNICODE
			wPosSpan = CalculatePosSpan(lpPYArray+(*lpwPYArrayCurPos)*MAX_PY_LEN,
					MAX_PY_LEN,(WORD)((*lpwPYArrayLen)-(*lpwPYArrayCurPos)),(WORD)_tcslen(szTempStr));
#else
			wPosSpan = CalculatePosSpan(lpPYArray+(*lpwPYArrayCurPos)*MAX_PY_LEN,
					MAX_PY_LEN,(WORD)((*lpwPYArrayLen)-(*lpwPYArrayCurPos)),(WORD)(_tcslen(szTempStr)/2));
#endif
			*lpwPYArrayCurPos += wPosSpan;

			*lpwUnConvPos = CalculateUnConvPos(lpPYArray + (*lpwPYArrayCurPos)*MAX_PY_LEN,
						MAX_PY_LEN,(WORD)((*lpwPYArrayLen)-(*lpwPYArrayCurPos)),lpStr);

			*lpwEditCaret = (SHORT)_tcslen(lpStr);

			ConvertPY( lpPYArray + (*lpwPYArrayCurPos)*MAX_PY_LEN,
				MAX_PY_LEN,(WORD)((*lpwPYArrayLen)-(*lpwPYArrayCurPos)),lpPYCand);

		    lpPYCand->awBeforePos[1] = 0;
			lpPYCand->awBeforePos[0] = 0;
			lpPYCand->awCurrentPos[1] = 0;
			lpPYCand->awCurrentPos[0] = 0;
			lpPYCand->wSelectDirect = SELECT_FORWARD;
			lpPYCand->wSelectStatus = 1;

			lpCandList->dwCount = CreateCandStr(lpPYCand, SELECT_FORWARD, (LPTSTR)(((LPMYCAND)lpCandInfo)->szCandStr),MAXCANDSTRSIZE);
			lpCandList->dwPageStart = 2;
			lpCandList->dwPageSize = 10;

			CreatePaintStr( lpPYArray + (*lpwPYArrayCurPos)*MAX_PY_LEN,
				MAX_PY_LEN,(WORD)((*lpwPYArrayLen)-(*lpwPYArrayCurPos)),lpConvStr,lpPaintStr);
		}
    }
    GnMsg.msg = WM_IME_COMPOSITION;
    GnMsg.wParam = 0;
	GnMsg.lParam = GCS_COMPSTR;
    GenerateMessage(hIMC, lpdwCurTransKey,(LPGENEMSG)&GnMsg);
my_exit:
	ImmUnlockIMCC(lpIMC->hCandInfo);
    ImmUnlockIMCC(lpIMC->hCompStr);
    ImmUnlockIMC(hIMC);
	return TRUE;
}

BOOL DeletePhrase(HIMC hIMC,WORD wParam)
{
    LPINPUTCONTEXT lpIMC;
	LPCANDIDATEINFO lpCandInfo;
	LPCANDIDATELIST lpCandList;

	LPFREEPYCAND lpPYCand;
	LPKEYPH lpKPh;
	LPHZPH lpHZPh,lpCurHZPh,lpPreHZPh;

	WORD wLen,wIdx;
	WORD wRet = 0;

    if( !IsCompStr(hIMC) ) return FALSE;

	lpIMC = ImmLockIMC(hIMC);

	lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
	lpCandList = (LPCANDIDATELIST)((LPBYTE)lpCandInfo  + lpCandInfo->dwOffset[0]);
	lpPYCand = (LPFREEPYCAND)(&((LPMYCAND)lpCandInfo)->FreePYCand);

	if( ! lpPYCand->wSelectStatus ) {
		ImmUnlockIMCC(lpIMC->hCandInfo);
	    ImmUnlockIMC(hIMC);
		return FALSE;
	}

	switch( wParam ) {
	case _T('!'):
		wIdx = 0;
		break;
	case _T('@'):
		wIdx = 1;
		break;
	case _T('#'):
		wIdx = 2;
		break;
	case _T('$'):
		wIdx = 3;
		break;
	case _T('%'):
		wIdx = 4;
		break;
	case _T('^'):
		wIdx = 5;
		break;
	case _T('&'):
		wIdx = 6;
		break;
	case _T('*'):
		wIdx = 7;
		break;
	case _T('('):
		wIdx = 8;
		break;
	case _T(')'):
		wIdx = 9;
		break;
	default:
		wIdx = 0;
		break;
	}

    switch(lpPYCand->wSelectDirect){
    case SELECT_FORWARD:
		wIdx += lpPYCand->awBeforePos[lpPYCand->wSelectStatus];
		break;
    case SELECT_BACKWARD:
		wIdx += lpPYCand->awCurrentPos[lpPYCand->wSelectStatus]+1;
		break;
    default:
		break;
    }
	wLen = lpPYCand->wPhraseNum;

    if(0 < wIdx < wLen ){
		lpKPh = (lpPYCand->aPhrase+wIdx)->lpKeyPH;
		lpHZPh = (lpPYCand->aPhrase+wIdx)->lpHZPH;
		if( ((LPBYTE)lpKPh >= (LPBYTE)lpMapFileBase 
			&& (LPBYTE)lpKPh < (LPBYTE)lpMapFileBase + MAX_PY_NUM*sizeof(KEYPH) ) ||
			((LPBYTE)lpKPh >= (LPBYTE)lpMapFileBase + 2*MAX_PY_NUM*sizeof(KEYPH)
			&&(LPBYTE)lpKPh < (LPBYTE)lpMapFileBase + dwMapFileUsrOffset ) ) {
			lpHZPh->dwAttrib = 0;
			wRet = 1;
		}
		else{
			lpCurHZPh = lpKPh->lpHZPH;
			if( lpCurHZPh == lpHZPh) {
				if(lpHZPh->lpNext == NULL) lpKPh->wLen = -1;
				lpKPh->lpHZPH = lpHZPh->lpNext;
				wRet = 1;
			}
			else {
				lpPreHZPh = lpCurHZPh;
				do {
					lpCurHZPh = lpCurHZPh->lpNext;
					if(lpCurHZPh == lpHZPh) {
						lpPreHZPh->lpNext = lpCurHZPh->lpNext;
						wRet = 1;
						break;
					}
					lpPreHZPh = lpCurHZPh;
				}while(lpCurHZPh->lpNext != NULL);
			}
		}
	}
	if( wRet )	MakeResultString(hIMC,FALSE);
	ImmUnlockIMCC(lpIMC->hCandInfo);
    ImmUnlockIMC(hIMC);
	return TRUE;
}