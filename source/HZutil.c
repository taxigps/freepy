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

WORD ParsePY(LPTSTR lpPY, LPTSTR lpTranBuf, WORD wMaxPYLen)
{
	LPPINYIN lpPYTab = (LPPINYIN)aPYTab;
	int i,j;
	WORD wHead,wPYLen,wOffset=0,wCount=1,wTotal=0;
	int flag=0,eqflag=0,slflag=0,yhflag=0,yunflag=0,rflag=0;
	TCHAR szStr[10],szTmpStr[10],szSlStr[3][3];
	TCHAR szYun[7];
	int nTemp;
	LPTSTR pDest;

	wPYLen = _tcslen(lpPY);
	if( wPYLen < 1 || wPYLen > MAX_PHRASE_LEN*wMaxPYLen ) return 0;
	
	if(wPYLen == 1){
		if(*lpPY >= _T('a') && *lpPY <= _T('z')){
			_tcscpy(lpTranBuf,lpPY);
			return 1;
		}
		else return 0;
	}  
	szYun[0] = _T('a');
	szYun[1] = _T('o');
	szYun[2] = _T('e');
	szYun[3] = _T('i');
	szYun[4] = _T('u');
	szYun[5] = _T('v');
	szYun[6] = _T('\0');

	szSlStr[0][0] = _T('c');
	szSlStr[1][0] = _T('s');
	szSlStr[2][0] = _T('z');
	for(i=0;i<3;i++){
		szSlStr[i][1] = _T('h');
		szSlStr[i][2] = _T('\0');
	}
	
	while(1){
		wHead = *(lpPY+wOffset) - _T('a');
		if((wHead < 0 || wHead > 25) && *(lpPY+wOffset) != _T('\'')){
			return 0;
		}
		if(*(lpPY+wOffset) == _T('\'')){
			if(wTotal > 0 && *(lpTranBuf + (wTotal-1)*wMaxPYLen) >= _T('a')
				&& *(lpTranBuf + (wTotal-1)*wMaxPYLen) <= _T('z') ){
				_tcscpy(lpTranBuf+((wTotal)++)*wMaxPYLen,_T("\'"));
			}
			wOffset++;
			if(wOffset == wPYLen){
				return wTotal;
			}
			continue;
		}
		wCount=1;
		while((wOffset+wCount) <= wPYLen){
			_tcsncpy(szStr,lpPY+wOffset,wCount);
			szStr[wCount] = _T('\0');
			if(wCount == 1) flag=1;
			else{
				flag=0;
				for(i=0;(lpPYTab+wHead*MAX_EACH_PY_NUM+i)->wKey;i++){
					if(_tcsstr( (lpPYTab+wHead*MAX_EACH_PY_NUM+i)->szPY,szStr)!=NULL){
						flag=1;
						break;
					}
				}
			}
			
			if( (wOffset+wCount) == wPYLen ){
				if(flag) {
					_tcscpy(lpTranBuf+((wTotal)++)*wMaxPYLen,szStr);
					return wTotal;
				}
				else {
					if(wCount==2){
						_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr,wCount-1);
						*(lpTranBuf+((wTotal)++)*wMaxPYLen+wCount-1)=_T('\0');
						_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr+wCount-1,1);
						*(lpTranBuf+((wTotal)++)*wMaxPYLen+1)=_T('\0');
						return wTotal;
					}
					nTemp = szStr[wCount-1];
					_tcsncpy(szTmpStr,szStr,wCount-1);
					szTmpStr[wCount-1]='\0';
					eqflag=0;
					for(i=0;(lpPYTab+wHead*MAX_EACH_PY_NUM+i)->wKey;i++){
						if( !_tcscmp((lpPYTab+wHead*MAX_EACH_PY_NUM+i)->szPY,szTmpStr) ){
							eqflag=1;
							break;
						}
					}
					if(eqflag){
						yunflag = 0;
						if( _tcsrchr(szYun,szStr[wCount-2]) == NULL && _tcsrchr(szYun,nTemp) != NULL) {
							szTmpStr[_tcslen(szTmpStr)-1] = _T('\0');
							for(i=0;(lpPYTab+wHead*MAX_EACH_PY_NUM+i)->wKey;i++){
								if( !_tcscmp((lpPYTab+wHead*MAX_EACH_PY_NUM+i)->szPY,szTmpStr) ){
									goto my_next1;
								}
							}
						}
						goto my_next2;
my_next1:
						_tcscpy(szTmpStr,szStr + _tcslen(szStr) -2);
						for(i=0;(lpPYTab+(szTmpStr[0] - _T('a'))*MAX_EACH_PY_NUM+i)->wKey;i++){
							if( !_tcscmp((lpPYTab+(szTmpStr[0] - _T('a'))*MAX_EACH_PY_NUM+i)->szPY,szTmpStr) ){
								yunflag = 1;
								break;
							}
						}

my_next2:
						if(yunflag){
							_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr,wCount-2);
							*(lpTranBuf+((wTotal)++)*wMaxPYLen+wCount-2)=_T('\0');
							_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr+wCount-2,2);
							*(lpTranBuf+((wTotal)++)*wMaxPYLen+2)=_T('\0');
						}
						else {
							_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr,wCount-1);
							*(lpTranBuf+((wTotal)++)*wMaxPYLen+wCount-1)=_T('\0');
							_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr+wCount-1,1);
							*(lpTranBuf+((wTotal)++)*wMaxPYLen+1)=_T('\0');
						}
						return wTotal;
					}	   
					else{
						for(i=wCount-2 ;i;i--){
							_tcsncpy(szTmpStr,szStr,i);
							szTmpStr[i]=_T('\0');
							rflag = 0;
							for(j=0;(lpPYTab+wHead*MAX_EACH_PY_NUM+j)->wKey;j++){
								if( !_tcscmp((lpPYTab+wHead*MAX_EACH_PY_NUM+j)->szPY,szTmpStr) ){
									rflag = 1;
									break;
								}
							}
							if(rflag) break;
						}
						if(rflag){
							wOffset += i;
							_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr,i);
							*(lpTranBuf+((wTotal)++)*wMaxPYLen+i)=_T('\0');
							break;
						}
						_tcsncpy(szTmpStr,szStr,2);
						szTmpStr[2]=_T('\0');
						slflag=0;
						
						for(i=0;i<3;i++){
							if( !_tcscmp(szTmpStr,szSlStr[i]) ){
								slflag=1;
								break;
							}
						}
						if(slflag){
							wOffset += 2;
							_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr,2);
							*(lpTranBuf+((wTotal)++)*wMaxPYLen+2)=_T('\0');
							break;
						}
						else{
							wOffset++;
							_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr,1);
							*(lpTranBuf+((wTotal)++)*wMaxPYLen+1)=_T('\0');
							break;
						}
					}
				}
			}
			
			if(flag) wCount++;
			else {
				if(wCount==2){
					wOffset++;
					_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr,wCount-1);
					*(lpTranBuf+((wTotal)++)*wMaxPYLen+wCount-1)=_T('\0');
					break;
				}
				nTemp = szStr[wCount-1];
				_tcsncpy(szTmpStr,szStr,wCount-1);
				szTmpStr[wCount-1]='\0';
				eqflag=0;
				for(i=0;(lpPYTab+wHead*MAX_EACH_PY_NUM+i)->wKey;i++){
					if( !_tcscmp((lpPYTab+wHead*MAX_EACH_PY_NUM+i)->szPY,szTmpStr) ){
						eqflag=1;
						break;
					}
				}
				if(eqflag){
					yunflag = 0;
					if( _tcsrchr(szYun,szStr[wCount-2]) == NULL && _tcsrchr(szYun,nTemp) != NULL) {
						szTmpStr[_tcslen(szTmpStr)-1] = _T('\0');
						for(i=0;(lpPYTab+wHead*MAX_EACH_PY_NUM+i)->wKey;i++){
							if( !_tcscmp((lpPYTab+wHead*MAX_EACH_PY_NUM+i)->szPY,szTmpStr) ){
								goto my_next3;
							}
						}
					}
					goto my_next4;
my_next3:
					_tcscpy(szTmpStr,szStr + _tcslen(szStr) -2);
					for(i=0;(lpPYTab+(szTmpStr[0] - _T('a'))*MAX_EACH_PY_NUM+i)->wKey;i++){
						if( (pDest = _tcsstr((lpPYTab+(szTmpStr[0] - _T('a'))*MAX_EACH_PY_NUM+i)->szPY,szTmpStr)) != NULL ){
							if(pDest == (lpPYTab+(szTmpStr[0] - _T('a'))*MAX_EACH_PY_NUM+i)->szPY) {
								yunflag = 1;
								break;
							}
						}
					}
my_next4:

					if(yunflag) {
						wOffset += wCount-2;
						_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr,wCount-2);
						*(lpTranBuf+((wTotal)++)*wMaxPYLen+wCount-2)=_T('\0');
					}
					else {
						wOffset += wCount-1;
						_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr,wCount-1);
						*(lpTranBuf+((wTotal)++)*wMaxPYLen+wCount-1)=_T('\0');
					}
				}
				else{
					for(i=wCount-2 ;i;i--){
						_tcsncpy(szTmpStr,szStr,i);
						szTmpStr[i]=_T('\0');
						rflag = 0;
						for(j=0;(lpPYTab+wHead*MAX_EACH_PY_NUM+j)->wKey;j++){
							if( !_tcscmp((lpPYTab+wHead*MAX_EACH_PY_NUM+j)->szPY,szTmpStr) ){
								rflag = 1;
								break;
							}
						}
						if(rflag) break;
					}
					if(rflag){
						wOffset += i;
						_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr,i);
						*(lpTranBuf+((wTotal)++)*wMaxPYLen+i)=_T('\0');
						break;
					}

					_tcsncpy(szTmpStr,szStr,2);
					szTmpStr[2]=_T('\0');
					slflag=0;
					
					for(i=0;i<3;i++){
						if( !_tcscmp(szTmpStr,szSlStr[i]) ){
							slflag=1;
							break;
						}
					}
					if(slflag){
						wOffset += 2;
						_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr,2);
						*(lpTranBuf+((wTotal)++)*wMaxPYLen+2)=_T('\0');
					}
					else{
						wOffset++;
						_tcsncpy(lpTranBuf+wTotal*wMaxPYLen,szStr,1);
						*(lpTranBuf+((wTotal)++)*wMaxPYLen+1)=_T('\0');
					}
				}
				break;
			}
		}
	}
}
	
void SortPhrase(LPPHRASE lpPh, WORD wLen)
{
	int i,j;
	PHRASE TempPh;
	
	for(i=0;i<wLen;i++){
		for(j=i;j<wLen;j++){
			if( (lpPh+i)->lpHZPH->dwAttrib < (lpPh+j)->lpHZPH->dwAttrib){
				TempPh = *(lpPh+i);
				*(lpPh+i)=*(lpPh+j);
				*(lpPh+j)=TempPh;
			}
		}
	}
}

WORD CreatePYKey(LPTSTR lpPYArray,WORD wMaxPYLen,WORD wPYArrayLen,WORD awPYArrayKey[][2*MAX_EACH_PY_NUM+1])
{
	LPPINYIN lpPYTab = (LPPINYIN)aPYTab;
	LPPINYIN lpPYTemp;
	TCHAR szSlStr[3][3],szPY[4][MAX_PY_LEN],szBStr[3][4],szTemp[4];
	WORD awSlKey[3][MAX_EACH_PY_NUM],awNmKey[3][MAX_EACH_PY_NUM];
	WORD wHead,wCount,wKeyLen,wTemp,wTemp1,wTempKey;
	WORD wSlPos,wFlag;
	TCHAR cTempChar;
	LPTSTR lpDest;
	int i,j,k;
	
	szBStr[0][0] = _T('a');
	szBStr[1][0] = _T('e');
	szBStr[2][0] = _T('i');
	for(i=0;i<3;i++){
		szBStr[i][1] = _T('n');
		szBStr[i][2] = _T('g');
		szBStr[i][3] = _T('\0');
	}

	szSlStr[0][0] = _T('c');
	szSlStr[1][0] = _T('s');
	szSlStr[2][0] = _T('z');
	for(i=0;i<3;i++){
		szSlStr[i][1] = _T('h');
		szSlStr[i][2] = _T('\0');
	}
	
	for(i=0;i<3;i++){
		wTemp = 0;
		wTemp1 = 0;
		for(j=0;(lpPYTemp=(lpPYTab+ (szSlStr[i][0] - _T('a'))*MAX_EACH_PY_NUM+j))->wKey;j++){
			if( _tcsstr(lpPYTemp->szPY,szSlStr[i] ) != NULL )
				awSlKey[i][wTemp++] = lpPYTemp->wKey;
			else
				awNmKey[i][wTemp1++] = lpPYTemp->wKey;
		}
		awSlKey[i][wTemp] = 0;
		awNmKey[i][wTemp1] = 0;
	}

	wCount = 0;
	for(i=0;i<wPYArrayLen;i++){
		if((cTempChar = *(lpPYArray + i*wMaxPYLen))== _T('i') || 
				cTempChar == _T('u') || cTempChar == _T('v') ) 
			continue;

		_tcscpy(szPY[0],lpPYArray + i*wMaxPYLen);
		if(szPY[0][0] < _T('a') || szPY[0][0] > _T('z') ) continue;

		szPY[1][0] = _T('\0');
		szPY[2][0] = _T('\0');
		szPY[3][0] = _T('\0');

		if( wConversionSet & CONVERSION_SET_FUZZYC ) {
			if( (cTempChar = szPY[0][0]) == _T('c') ||
				cTempChar == _T('s') || cTempChar == _T('z') ) {
				if( szPY[0][1] == _T('h') ) {
					szPY[1][0] = cTempChar;
					_tcscpy(szPY[1]+1,szPY[0] + 2);
				}
				else{
					szPY[1][0] = cTempChar;
					szPY[1][1] = _T('h');
					_tcscpy(szPY[1]+2,szPY[0] + 1);
				}
			}
		}
		if( wConversionSet & CONVERSION_SET_FUZZYN ) {
			if( szPY[0][0] == _T('n') ) {
				szPY[1][0] = _T('l');
				_tcscpy(szPY[1]+1,szPY[0] + 1);
			}
			else if(szPY[0][0] == _T('l') ) {
				szPY[1][0] = _T('n');
				_tcscpy(szPY[1]+1,szPY[0] + 1);
			}
		}

		if( wConversionSet & CONVERSION_SET_FUZZYH ) {
			if( szPY[0][0] == _T('h') ) {
				szPY[1][0] = _T('f');
				_tcscpy(szPY[1]+1,szPY[0] + 1);
			}
			else if(szPY[0][0] == _T('f') ) {
				szPY[1][0] = _T('h');
				_tcscpy(szPY[1]+1,szPY[0] + 1);
			}
		}

		if( wConversionSet & CONVERSION_SET_FUZZYB ) {
			for(i=0;i<2;i++){
				wFlag = 0;
				for(j=0;j<3;j++){
					if( (lpDest = _tcsstr(szPY[i],szBStr[j])) != NULL){
						if( (szPY[i] + _tcslen(szPY[i]) - _tcslen(szBStr[j])) == lpDest) {
							wFlag = 1;
							break;
						}
					}
				}
				if(wFlag){
					_tcscpy(szPY[i+2],szPY[i]);
					szPY[i+2][_tcslen(szPY[i+2])-1] = _T('\0');
				}
			}
			for(i=0;i<2;i++){
				wFlag = 0;
				for(j=0;j<3;j++){
					_tcscpy(szTemp,szBStr[j]);
					szTemp[_tcslen(szTemp)-1] = _T('\0');
					if( (lpDest = _tcsstr(szPY[i],szTemp)) != NULL){
						if( (szPY[i] + _tcslen(szPY[i]) - _tcslen(szTemp)) == lpDest) {
							wFlag = 1;
							break;
						}
					}
				}
				if(wFlag){
					_tcscpy(szPY[i+2],szPY[i]);
					szPY[i+2][_tcslen(szPY[i+2])+1] = _T('\0');
					szPY[i+2][_tcslen(szPY[i+2])] = _T('g');
				}
			}
		}

		wKeyLen = 0;
		for(k = 0;k < 4;k++) {
			wHead = (WORD)(szPY[k][0] - _T('a'));
			if( _tcslen(szPY[k]) == 0 ) continue;
			if( _tcslen(szPY[k]) == 1){
				if( (cTempChar = szPY[k][0]) == _T('c') ||
					cTempChar == _T('s') || cTempChar == _T('z') ) {
					if( cTempChar == _T('c') ) wTemp = 0;
					else if( cTempChar == _T('s') ) wTemp = 1;
					else if( cTempChar == _T('z') ) wTemp = 2;
					else wTemp = 0;
					for(j=0;awNmKey[wTemp][j];j++){
						awPYArrayKey[wCount][wKeyLen++] = awNmKey[wTemp][j];
					}
				}
				else {
					for(j=0;wTempKey=((lpPYTab + wHead*MAX_EACH_PY_NUM +j)->wKey);j++)
						awPYArrayKey[wCount][wKeyLen++]=wTempKey;
				}
			}
			else{
				wFlag = 0;
				for(j=0;j<3;j++){
					if( !_tcscmp(szPY[k],szSlStr[j]) ){
						wFlag=1;
						wSlPos=j;
						break;
					}
				}
				if(wFlag){
					for(j=0;awSlKey[wSlPos][j];j++){
						awPYArrayKey[wCount][wKeyLen++] = awSlKey[wSlPos][j];
					}
				}
				else {
					for(j=0;(lpPYTab + wHead*MAX_EACH_PY_NUM+j)->wKey;j++){
						if( !_tcscmp(szPY[k],(lpPYTab + wHead*MAX_EACH_PY_NUM +j)->szPY)){
							awPYArrayKey[wCount][wKeyLen++] = (lpPYTab + wHead*MAX_EACH_PY_NUM+j)->wKey;
							break;
						}
					}
				}
			}
		}
		awPYArrayKey[wCount][wKeyLen]=0;
		wCount++;
	}
	return wCount;
}

void ConvertPY(LPTSTR lpPYArray,WORD wMaxPYLen,WORD wPYArrayLen,LPFREEPYCAND lpPYCand)
{
	LPHANZI lpHanZi = lpPYCand->aHanZi;
	LPPHRASE lpPhrase = lpPYCand->aPhrase;

	WORD awPYArrayKey[MAX_PHRASE_LEN][2*MAX_EACH_PY_NUM+1];
	BYTE abKey[MAX_PHRASE_LEN+1];
	BYTE abKeyArray[2][MAX_SELECT_PH*5][MAX_PHRASE_LEN+1];
	WORD awKeyArrayLen[2];
	LPKEYPH	alpKeyPh[MAX_SELECT_PH];
	LPKEYPH alpTempKeyPh[2];
	PHRASE aTempPh[MAX_SELECT_PH];
	WORD wResult,wTotal,wHZLen;	
	WORD wCount,wTemp;
	int i,j,k;
	WORD wPhLen,wTotalPh=0;

	if(wPYArrayLen < 1){
		lpPYCand->wPhraseNum = 0;
		lpPYCand->wHZNum = 0;
		return;
	}

	wCount = CreatePYKey(lpPYArray,wMaxPYLen,wPYArrayLen,awPYArrayKey);

	awKeyArrayLen[0]=0;
	awKeyArrayLen[1]=0;
	
	wHZLen = 0;
	wTotal = 0;
	for(k=0;awPYArrayKey[0][k];k++){
		wHZLen += _tcslen(*(alpHZTab+(awPYArrayKey[0][k]-1)));
		lpHanZi->lpHZ = *(alpHZTab+(awPYArrayKey[0][k]-1));
		lpHanZi->wKey = awPYArrayKey[0][k];

#ifdef _UNICODE
		(lpHanZi++)->wLen = wHZLen;
#else
		(lpHanZi++)->wLen = wHZLen/2;
#endif	
		
		abKey[1] = (BYTE)( awPYArrayKey[0][k] & 0xff);
		abKey[0] = 0;
		abKey[0] |= (BYTE)(( awPYArrayKey[0][k] & 0x0100) >> 8);
		
		wResult=QueryPhrase(abKey,1,alpTempKeyPh);
		if( alpTempKeyPh[0] != NULL) *(alpKeyPh + (wTotal++)) = alpTempKeyPh[0];
		if( alpTempKeyPh[1] != NULL) *(alpKeyPh + (wTotal++)) = alpTempKeyPh[1];
		if(wResult > 0)
			memcpy(abKeyArray[0][awKeyArrayLen[0]++],abKey,2);
	}
	
	lpPYCand->wHZNum = lpHanZi - lpPYCand->aHanZi;
	
#define TOGGLE(i) ( (i%2) ? 0 : 1 )	
	for(i=1;i<wCount;i++){
		for(j=0;j<awKeyArrayLen[TOGGLE(i)];j++){
			for(k=0;awPYArrayKey[i][k];k++){
				memcpy(abKey,abKeyArray[TOGGLE(i)][j],i+1);
				abKey[i+1] = (BYTE) ( awPYArrayKey[i][k] & 0xff);
				abKey[0] |= (BYTE)(( awPYArrayKey[i][k] & 0x0100) >> (8-i));

				wResult=QueryPhrase(abKey,(WORD)(i+1),alpTempKeyPh);
				if( alpTempKeyPh[0] != NULL) *(alpKeyPh + (wTotal++)) = alpTempKeyPh[0];
				if( alpTempKeyPh[1] != NULL) *(alpKeyPh + (wTotal++)) = alpTempKeyPh[1];	
				if(wResult > 0)
					memcpy(abKeyArray[i%2][awKeyArrayLen[i%2]++],abKey,i+2);
			}
		}
		awKeyArrayLen[TOGGLE(i)] = 0;
	}

#undef TOGGLE

	if(wTotal > 0){
		wTotalPh = 0;
		wPhLen = 0;
		wTemp = alpKeyPh[wTotal-1]->wLen;
		
		for(i=wTotal-1;i+1;i--){
			if(alpKeyPh[i]->wLen == wTemp){
				aTempPh[wPhLen].lpHZPH = alpKeyPh[i]->lpHZPH;
				aTempPh[wPhLen].lpKeyPH = alpKeyPh[i];
				wPhLen++;
				
				while(aTempPh[wPhLen-1].lpHZPH->lpNext != NULL){
					aTempPh[wPhLen].lpHZPH = aTempPh[wPhLen-1].lpHZPH->lpNext;
					aTempPh[wPhLen].lpKeyPH = alpKeyPh[i];
					wPhLen++;
				}
				wTemp = alpKeyPh[i]->wLen;
				if( i==0 ){
					if( wConversionSet & CONVERSION_SET_SORT)
						SortPhrase(aTempPh,wPhLen);
					memcpy(lpPhrase + wTotalPh,aTempPh,wPhLen*sizeof(PHRASE));
					wTotalPh += wPhLen;
				}
			}
			else{
				if( wConversionSet & CONVERSION_SET_SORT)
					SortPhrase(aTempPh,wPhLen);
				memcpy(lpPhrase + wTotalPh,aTempPh,wPhLen*sizeof(PHRASE));
				wTotalPh += wPhLen;
				wPhLen=0;
				wTemp = alpKeyPh[i]->wLen;
				i++;
			}
		}
	}
	lpPYCand->wPhraseNum = wTotalPh;
	return;
}

WORD CreateCandStr(LPFREEPYCAND lpPYCand, WORD wDirect, LPTSTR lpCandStr,WORD wMaxCandStrSize)
{
	LPHANZI lpHanZi = lpPYCand->aHanZi;
	LPPHRASE lpPhrase = lpPYCand->aPhrase;
	WORD *lpwStatus = &(lpPYCand->wSelectStatus);
	SHORT *lpwCurPos = lpPYCand->awCurrentPos;
	SHORT *lpwBfPos = lpPYCand->awBeforePos;
	WORD awLen[2];
	WORD wCount=0,wTemp,wTempPos,wTempBfPos;
	TCHAR szTemp[30],szTotal[150];
	TCHAR szHZStr[2*MAX_PHRASE_LEN+10],szStrBuf[CAND_PAGESIZE+4][2*MAX_PHRASE_LEN+10];
	int i,flag;
	HDC hDC;
	SIZE sz;

	awLen[1] = lpPYCand->wPhraseNum;
	awLen[0] = (lpPYCand->aHanZi + lpPYCand->wHZNum - 1)->wLen;

	DebugLog(0,(DebugLogFile,"CreateCandStr: %d  %d  %d\n",awLen[1],awLen[0],*lpwStatus));	

	if( *lpwStatus && awLen[*lpwStatus] < 1){
		*lpwStatus = 0;
	}
	
	if(!(*lpwStatus) && awLen[*lpwStatus]<1) return wCount;
	hDC = GetDC(NULL);
	_tcscpy(szTotal,"<>");

	switch(wDirect){
	case SELECT_FORWARD:
		if( !(lpPYCand->wSelectDirect & wDirect) ){
			wTempPos = *(lpwCurPos+(*lpwStatus));
			*(lpwCurPos+(*lpwStatus)) = *(lpwBfPos+(*lpwStatus))+1;
			*(lpwBfPos+(*lpwStatus)) = wTempPos+1;
			lpPYCand->wSelectDirect = wDirect;
		}
		
		if( *(lpwCurPos+(*lpwStatus)) > awLen[*lpwStatus] - 1){
			if(!(*lpwStatus)){
				MessageBeep(0xFFFFFFFF );
				return wCount;
			}
			else{
				*lpwStatus = 0;
				*(lpwBfPos+(*lpwStatus)) = 0;
				*(lpwCurPos+(*lpwStatus)) = 0;
			}
		}
		
		if(*(lpwCurPos+(*lpwStatus)) < 0) *(lpwCurPos+(*lpwStatus)) = 0;
		
		if((!(*lpwStatus) && awLen[1]>0) || *(lpwCurPos+(*lpwStatus)) > 0)
			_tcscpy( lpCandStr , _T("<") );
		else
			_tcscpy( lpCandStr , _T("") );

		*(lpwBfPos+(*lpwStatus)) = *(lpwCurPos+(*lpwStatus));
		wCount = 2;
		while( *(lpwCurPos+(*lpwStatus)) < awLen[*lpwStatus] && wCount <= CAND_PAGESIZE+1) {
			if(*lpwStatus){
				_tcscpy( szHZStr,(lpPhrase+(*(lpwCurPos+(*lpwStatus))))->lpHZPH->szHZ );
			}
			else{
				for(i=0;i<lpPYCand->wHZNum;i++){
					if((lpHanZi+i)->wLen > *(lpwCurPos+(*lpwStatus)) ) break;
				}
#ifdef _UNICODE
				if(i)
					_tcsncpy(szHZStr,(lpHanZi+i)->lpHZ+(*(lpwCurPos+(*lpwStatus))-(lpHanZi+i-1)->wLen),1);
				else
					_tcsncpy(szHZStr,(lpHanZi+i)->lpHZ+(*(lpwCurPos+(*lpwStatus))),1);
				szHZStr[1]=_T('\0');
#else
				if(i)
					_tcsncpy(szHZStr,(lpHanZi+i)->lpHZ+2*(*(lpwCurPos+(*lpwStatus))-(lpHanZi+i-1)->wLen),2);
				else
					_tcsncpy(szHZStr,(lpHanZi+i)->lpHZ+2*(*(lpwCurPos+(*lpwStatus))),2);
				szHZStr[2]=_T('\0');
#endif
			}

			_stprintf(szTemp,"%d%s ",wCount%10,szHZStr);
			
			if(wConversionSet & CONVERSION_SET_GBK) {
				if(wConversionSet & CONVERSION_SET_SHAPE) {
					_tcscat(szTotal,szTemp);
					GetTextExtentPoint(hDC,szTotal,_tcslen(szTotal),&sz);
					if(sz.cx > sizeCand[1].cx) break;
				}
				_tcscpy( (lpCandStr + (wCount++)*wMaxCandStrSize),szHZStr);
			}
			else {
				if(*lpwStatus){
					if(wConversionSet & CONVERSION_SET_SHAPE) {
						_tcscat(szTotal,szTemp);
						GetTextExtentPoint(hDC,szTotal,_tcslen(szTotal),&sz);
						if(sz.cx > sizeCand[1].cx) break;
					}
					_tcscpy( (lpCandStr + (wCount++)*wMaxCandStrSize),szHZStr);
				}
				else{
					if( (*((LPBYTE)szHZStr) >= GB_QUFIRST && *((LPBYTE)szHZStr) <= GB_QULAST) && 
						(*((LPBYTE)szHZStr+1) >= GB_WEIFIRST && *((LPBYTE)szHZStr+1) <= GB_WEILAST) ) {
						if(wConversionSet & CONVERSION_SET_SHAPE) {
							_tcscat(szTotal,szTemp);
							GetTextExtentPoint(hDC,szTotal,_tcslen(szTotal),&sz);
							if(sz.cx > sizeCand[1].cx) break;
						}
						_tcscpy( (lpCandStr + (wCount++)*wMaxCandStrSize),szHZStr);
					}
				}
			}
			*(lpwCurPos+(*lpwStatus)) += 1;
		}
		
		if(*lpwStatus || *(lpwCurPos+(*lpwStatus)) < awLen[*lpwStatus] - 1 && wCount > 1){
			_tcscpy( (lpCandStr + wMaxCandStrSize),_T(">"));
		}
		else {
			_tcscpy( (lpCandStr + wMaxCandStrSize),_T(""));
		}
		
		break;
		
	case SELECT_BACKWARD:
		if( (*lpwStatus && !(*(lpwBfPos+(*lpwStatus))))
			|| (!(*lpwStatus) && awLen[1]<1 && !(*(lpwBfPos+(*lpwStatus)))) ){
			MessageBeep(0xFFFFFFFF );
			return wCount;
		}
		if( !(lpPYCand->wSelectDirect & wDirect) ){
			*(lpwCurPos+(*lpwStatus)) = *(lpwBfPos+(*lpwStatus))-1;
			lpPYCand->wSelectDirect = wDirect;
		}
		
		flag=0;
		if( *(lpwCurPos+(*lpwStatus)) < 0 ){
			if(*lpwStatus || awLen[1] < 1 ){
				MessageBeep(0xFFFFFFFF );
				return wCount;
			}
			else{
				*lpwStatus = 1;
				flag = 1;
				wTempBfPos = *(lpwBfPos+1);
			}
		}
		
		if(flag && *(lpwCurPos+(*lpwStatus)) < awLen[*lpwStatus]-1)
			*(lpwCurPos+(*lpwStatus)) = awLen[*lpwStatus] - 1;

		if( *(lpwCurPos+(*lpwStatus)) > awLen[*lpwStatus]-1)
			*(lpwCurPos+(*lpwStatus)) = awLen[*lpwStatus] - 1;

		if( *lpwStatus || *(lpwCurPos+(*lpwStatus)) < awLen[*lpwStatus] -1)
			_tcscpy( szStrBuf[0], _T(">"));
		else szStrBuf[0][0]=_T('\0');
		
		*(lpwBfPos+(*lpwStatus)) = *(lpwCurPos+(*lpwStatus));

		wCount = 2;	
		while( *(lpwCurPos+(*lpwStatus)) >= 0  && wCount <= CAND_PAGESIZE+1) {
			if(*lpwStatus){
				_tcscpy( szStrBuf[wCount],(lpPhrase+(*(lpwCurPos+(*lpwStatus))))->lpHZPH->szHZ );
				_stprintf(szTemp,"%d%s ",wCount%10,(lpPhrase+(*(lpwCurPos+(*lpwStatus))))->lpHZPH->szHZ );
			}
			else{
				for(i=0;i<lpPYCand->wHZNum;i++){
					if((lpHanZi+i)->wLen > *(lpwCurPos+(*lpwStatus)) ) break;
				}
#ifdef _UNICODE
				if(i)
					_tcsncpy(szHZStr,(lpHanZi+i)->lpHZ+(*(lpwCurPos+(*lpwStatus))-(lpHanZi+i-1)->wLen),1);
				else
					_tcsncpy(szHZStr,(lpHanZi+i)->lpHZ+(*(lpwCurPos+(*lpwStatus))),1);
				szHZStr[1]=_T('\0');
				_tcscpy(szStrBuf[wCount],szHZStr);
#else
				if(i)
					_tcsncpy(szHZStr,(lpHanZi+i)->lpHZ+2*(*(lpwCurPos+(*lpwStatus))-(lpHanZi+i-1)->wLen),2);
				else
					_tcsncpy(szHZStr,(lpHanZi+i)->lpHZ+2*(*(lpwCurPos+(*lpwStatus))),2);
				szHZStr[2]=_T('\0');
				_stprintf(szTemp,"%d%s ",wCount%10,szHZStr);
				_tcscpy(szStrBuf[wCount],szHZStr);
#endif
			}

			if(flag && *(lpwCurPos+(*lpwStatus)) == wTempBfPos-1) break;

			if(wConversionSet & CONVERSION_SET_GBK) {
				if(wConversionSet & CONVERSION_SET_SHAPE) {
					_tcscat(szTotal,szTemp);
					GetTextExtentPoint(hDC,szTotal,_tcslen(szTotal),&sz);
					if(sz.cx > sizeCand[1].cx) break;
				}
				wCount++;
			}
			else {
				if(*lpwStatus) {
					if(wConversionSet & CONVERSION_SET_SHAPE) {
						_tcscat(szTotal,szTemp);
						GetTextExtentPoint(hDC,szTotal,_tcslen(szTotal),&sz);
						if(sz.cx > sizeCand[1].cx) break;
					}
					wCount++;
				}
				else{
					if( (*((LPBYTE)szHZStr) >= GB_QUFIRST && *((LPBYTE)szHZStr) <= GB_QULAST) && 
						(*((LPBYTE)szHZStr+1) >= GB_WEIFIRST && *((LPBYTE)szHZStr+1) <= GB_WEILAST) ) {
						if(wConversionSet & CONVERSION_SET_SHAPE) {
							_tcscat(szTotal,szTemp);
							GetTextExtentPoint(hDC,szTotal,_tcslen(szTotal),&sz);
							if(sz.cx > sizeCand[1].cx) break;
						}
						wCount++;
					}
				}
			}
			*(lpwCurPos+(*lpwStatus)) -= 1;
		}
		wTemp = wCount;

		if((!(*lpwStatus) && awLen[1] > 0 ) || (*(lpwCurPos+(*lpwStatus)) > 0 && wCount > 1))
			_tcscpy( lpCandStr, _T("<") );
		else
			_tcscpy( lpCandStr, _T("") );

		wCount = 2;
		for(i = wTemp-1;i>1;i--){
			_tcscpy( (lpCandStr + (wCount++)*wMaxCandStrSize),szStrBuf[i]);
		}
		if( _tcslen(szStrBuf[0])){
			_tcscpy( (lpCandStr + wMaxCandStrSize),szStrBuf[0]);
		}
		else{
			_tcscpy( (lpCandStr + wMaxCandStrSize),_T(""));
		}
		
		break;
		
	default:
		break; 
  }
  ReleaseDC(NULL,hDC);
  return wCount -2;
}

void CreatePaintStr(LPTSTR lpPYArray,WORD wMaxSize,WORD wLen,LPTSTR lpConvStr,LPTSTR lpPaintStr)
{
	
	int i;
	_tcscpy(lpPaintStr,lpConvStr);
	for(i=0;i<wLen;i++){
		_tcscat(lpPaintStr,lpPYArray+i*wMaxSize);
		if(i==wLen-1 || 
			*(lpPYArray+(i+1)*wMaxSize)==_T('\'')|| *(lpPYArray+i*wMaxSize)==_T('\''))
			continue;
		else
			_tcscat(lpPaintStr,_T(" "));
	}
	return;
}

WORD EffectPYArrayLen( LPTSTR lpPYArray,WORD wMaxSize,WORD wLen)
{
	int i;
	TCHAR ch;
	WORD wCount=0;
	
	for(i=0;i<wLen;i++){
		ch = *(lpPYArray + i * wMaxSize);
		if(ch == _T('i') || ch == _T('u') || ch == _T('v') ) continue;
		if(ch < _T('a') || ch > _T('z')) continue;
		wCount++;
	}
	return wCount;
}

WORD CalculatePosSpan(LPTSTR lpPYArray,WORD wMaxSize,WORD wLen,WORD wCount)
{
	WORD i;
	TCHAR ch;
	WORD wNum=0;
	
	for(i=0;i<wLen;i++){
		ch=*(lpPYArray + i*wMaxSize);
		if(ch == _T('i') || ch == _T('u') || ch == _T('v')) continue;
		if(ch < _T('a') || ch > _T('z')) continue;
		wNum++;
		if(wNum == wCount) return i+1;
	}
	return 0;
}

SHORT CalculateUnConvPos(LPTSTR lpPYArray,WORD wMaxSize,WORD wLen,LPTSTR lpStr)
{
	int i;
	TCHAR szStr[150];
	SHORT wPos = 0;
	LPTSTR lpDest;

	szStr[0] = _T('\0');
	for(i=0;i<wLen;i++)
		_tcscat(szStr,lpPYArray + i*wMaxSize);
	lpDest = _tcsstr(lpStr,szStr);
	if(lpDest)
		wPos = lpDest - lpStr;
	return wPos;
}
