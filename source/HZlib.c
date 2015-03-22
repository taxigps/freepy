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

void LoadHZDictionary( LPTSTR);
void LoadPunct( LPTSTR );
WORD String2Array(LPTSTR ,LPTSTR ,WORD );
WORD GetSegment(LPTSTR);
void GetStr(FILE *,LPTSTR);
void LoadTable();
void LoadPhrase();


void LoadHZDictionary( LPTSTR lpStr)
{
	TCHAR szPY[20],szHZ[1200];
	static int i=0,j=0,nPre=0;
	WORD wPYHead=1;
	LPPINYIN lpPYTab = (LPPINYIN)aPYTab;

	_stscanf(lpStr,"%s %s",szPY,szHZ);
	
	alpHZTab[i] = _tcsdup(szHZ);
	
	wPYHead=(WORD)szPY[0] - (WORD)_T('a');
	if(wPYHead != nPre) j=0;
	_tcscpy( (lpPYTab+wPYHead*MAX_EACH_PY_NUM+j)->szPY,szPY);
	(lpPYTab+wPYHead*MAX_EACH_PY_NUM+j)->wKey=i+1;

	nPre=wPYHead;
	i++,j++;
	return;
}

WORD String2Array(LPTSTR lpBuf,LPTSTR lpStrArr,WORD wMaxArrSize)
{
	int i;
	WORD cursor=0,count=0,wBufLen;
	
	wBufLen = strlen(lpBuf);
	
	for (i=0;i<wBufLen;i++){
		if(*(lpBuf+i) == _T(' ') || *(lpBuf+i) == _T('\t')) {
			if(i!=0 && *(lpBuf+i-1)!=_T(' ') && *(lpBuf+i-1)!=_T('\t') ){
				_tcsncpy(lpStrArr+count*wMaxArrSize,lpBuf+cursor,i-cursor);
				*(lpStrArr+count*wMaxArrSize+i-cursor)=_T('\0');
				count++;
			}
			cursor=i+1;
		}
		if(i == wBufLen-1 && *(lpBuf+i)!=_T(' ') && *(lpBuf+i)!=_T('	') ){
			_tcsncpy(lpStrArr+count*wMaxArrSize,lpBuf+cursor,i-cursor+1);
			*(lpStrArr+count*wMaxArrSize+i-cursor+1)=_T('\0');
			count++;
		}
	}
	return count;
}

void LoadPunct( LPTSTR lpStr)
{
	TCHAR szStrArr[70][70];
	WORD wCount,wHead;
	int i;
	
	wCount = String2Array(lpStr,(LPTSTR)szStrArr,70);

	if( szStrArr[0][0] > _T('~') || szStrArr[0][0] < _T('!')) return;

	wHead = szStrArr[0][0] - _T('!');

	for(i=1;i<wCount;i++){
		_tcscpy(aPunct[wHead][i-1],szStrArr[i]);
	}
	aPunct[wHead][wCount][0] = _T('\0');
}

WORD GetSegment(LPTSTR buf)
{
	if(*buf == _T('\0')) return 1; //END_SEGMENT
	else if(*buf == _T('#')) return 2; //COMMENT
	else if( _tcsstr(buf,_T("[PUNCTUATION]")) != NULL ) return 3; 
	else if( _tcsstr(buf,_T("[DICTIONARY]")) != NULL) return 4;
	else return 0;
}

void GetStr(FILE *pf,LPTSTR pbuf)
{
	while( !feof(pf) ) {
		*pbuf = _fgettc(pf);
		if(*pbuf == _T('\n')) break;
		pbuf++;
	}
	*pbuf = _T('\0');
}

void LoadTable()
{
	FILE *stream;
	TCHAR szStr[1200];
	TCHAR szTabFileName[200];
	LPTSTR lpTabFileName = szTabFileName;


	lpTabFileName += GetSystemDirectory(szTabFileName,200);
	if (*(lpTabFileName-1) != _T('\\'))
		*lpTabFileName++ = _T('\\');
	_tcscpy(lpTabFileName,TABFILENAME);

	if( (stream = _tfopen( szTabFileName, "r" )) == NULL ){
		_stprintf(szStr,"%s can not found",szTabFileName);
		MessageBox(NULL,szStr,"init",MB_OK);
		exit(1);
	}
	
	while( !feof( stream )) {
		GetStr(stream,szStr);

		switch( GetSegment(szStr)) {
		case 1: //END_SEGMENT
			break;

		case 2: //COMMENT
			break;

		case 3: //PUNCTUATION
			if( feof( stream ) ) goto my_exit;
			GetStr(stream,szStr);
			while(GetSegment(szStr) != 1) {
				if( GetSegment(szStr) != 2){
					LoadPunct( szStr );
				}
				if( feof( stream ) ) goto my_exit;
				GetStr(stream,szStr);
			}
			break;

		case 4: //DICTIONARY
			if( feof( stream ) ) goto my_exit;
			GetStr(stream,szStr);
			while(GetSegment(szStr) != 1) {
				if( GetSegment(szStr) != 2){
					LoadHZDictionary( szStr );
				}
				if( feof( stream ) ) goto my_exit;
				GetStr(stream,szStr);
			}
			break;

		default:
			break;
		}
	}
my_exit:
	fclose(stream);
	return;
}

void LoadPhrase()
{
	FILE *stream;
	BYTE bLen;
	WORD wLen;

	TCHAR szStr[2*(MAX_PHRASE_LEN+1)];
	BYTE abKey[MAX_PHRASE_LEN+1];

	WORD i;
	TCHAR szPhraseFileName[200],szTemp[200];
	LPTSTR lpPhraseFileName;

	for(i=0;i<2;i++){
		if(!i){
			lpPhraseFileName = szPhraseFileName;
			lpPhraseFileName += GetSystemDirectory(szPhraseFileName,200);
			if (*(lpPhraseFileName-1) != _T('\\'))
				*lpPhraseFileName++ = _T('\\');
			_tcscpy(lpPhraseFileName,SYSPHRASEFILENAME);

			if( (stream = _tfopen( szPhraseFileName, _T("rb") )) == NULL ){
				_stprintf(szTemp,"%s can not found",szPhraseFileName);
				MessageBox(NULL,szTemp,"DicInit",MB_OK);
				exit(1);
			}
		}
		else{
			lpPhraseFileName = szPhraseFileName;
			lpPhraseFileName += GetSystemDirectory(szPhraseFileName,200);
			if (*(lpPhraseFileName-1) != _T('\\'))
				*lpPhraseFileName++ = _T('\\');
			_tcscpy(lpPhraseFileName,USRPHRASEFILENAME);

			if( (stream = _tfopen( szPhraseFileName, _T("rb") )) == NULL ){
				_stprintf(szTemp,"%s can not found",szPhraseFileName);
//				MessageBox(NULL,szTemp,"DicInit",MB_OK);
				goto my_exit;
			}
		}
		
		while( !feof( stream )) {
			if(fread(&bLen,1,1,stream)){
				wLen=(WORD)bLen;
				if( wLen > 0 && fread(abKey,1,wLen+1,stream) &&
					fread(szStr,1,wLen*2,stream) ) {
					szStr[wLen*2] = _T('\0');
					SavePhToMapFile(szStr,abKey,wLen,i);
				}
			}
		}
		fclose(stream);
		if( !i )
			dwMapFileUsrOffset = 2*MAX_PY_NUM*sizeof(KEYPH) + dwMapFileOffset;
	}
my_exit:
	return;
}

void SavePhToMapFile(LPTSTR lpStr,LPBYTE lpbKey,WORD wLen,WORD wStatus)
{
	LPKEYPH lpKeyPH;
	LPHZPH lpHZPH;
	BOOL fFirst;
	WORD wHead;
	DWORD dwBaseOffset;

	if(wLen<1) return;

	dwBaseOffset = 2*MAX_PY_NUM*sizeof(KEYPH);

	wHead=(WORD)(*(lpbKey+1));
	wHead |= (WORD)((*lpbKey & 0x01) << 8);
	wHead--;
	if(wHead >= MAX_PY_NUM) return;

	if( !(lpKeyPH=(LPKEYPH)((LPBYTE)lpMapFileBase+wStatus*MAX_PY_NUM*sizeof(KEYPH))+wHead)->wLen ) {
		lpKeyPH->wLen=wLen;
		memcpy(lpKeyPH->abKey,lpbKey,wLen+1);
		lpKeyPH->lpNext=NULL;
		if((dwBaseOffset + dwMapFileOffset + sizeof(HZPH)) > MAPFILESIZE){
			return;
		}
		lpKeyPH->lpHZPH = (LPHZPH)((LPBYTE)lpMapFileBase + 
			dwBaseOffset + dwMapFileOffset);
		dwMapFileOffset += sizeof(HZPH);
		lpKeyPH->lpHZPH->dwAttrib=0;
		lpKeyPH->lpHZPH->lpNext=NULL;
		_tcscpy(lpKeyPH->lpHZPH->szHZ,lpStr);
	}
	
	else{
		fFirst=TRUE;
		do {
			if(fFirst)	fFirst=FALSE;
			else lpKeyPH=lpKeyPH->lpNext;
			
			if(lpKeyPH->wLen==wLen && !memcmp(lpKeyPH->abKey,lpbKey,wLen+1)){
				lpHZPH=lpKeyPH->lpHZPH;
				while(lpHZPH->lpNext != NULL)
					lpHZPH=lpHZPH->lpNext;

				if((dwBaseOffset + dwMapFileOffset + sizeof(HZPH)) > MAPFILESIZE){
					return;
				}
				lpHZPH->lpNext = (LPHZPH)((LPBYTE)lpMapFileBase + 
					dwBaseOffset + dwMapFileOffset);
				dwMapFileOffset += sizeof(HZPH);
				lpHZPH->lpNext->dwAttrib=0;
				lpHZPH->lpNext->lpNext=NULL;
				_tcscpy(lpHZPH->lpNext->szHZ,lpStr);
				goto my_exit;
			}
		}while(lpKeyPH->lpNext != NULL);

		if((dwBaseOffset + dwMapFileOffset + sizeof(KEYPH)) > MAPFILESIZE){
			return;
		}
		lpKeyPH->lpNext = (LPKEYPH)((LPBYTE)lpMapFileBase + 
			dwBaseOffset + dwMapFileOffset);
		dwMapFileOffset += sizeof(KEYPH);
		
		lpKeyPH->lpNext->wLen=wLen;
		memcpy(lpKeyPH->lpNext->abKey,lpbKey,wLen+1);
		lpKeyPH->lpNext->lpNext=NULL;

		if((dwBaseOffset + dwMapFileOffset + sizeof(HZPH)) > MAPFILESIZE){
			return;
		}
		lpKeyPH->lpNext->lpHZPH = (LPHZPH)((LPBYTE)lpMapFileBase + 
			dwBaseOffset + dwMapFileOffset);
		dwMapFileOffset += sizeof(HZPH);
		
		lpKeyPH->lpNext->lpHZPH->dwAttrib=0;
		lpKeyPH->lpNext->lpHZPH->lpNext=NULL;
		_tcscpy(lpKeyPH->lpNext->lpHZPH->szHZ,lpStr);
	}
my_exit:
	return;
}

WORD QueryPhrase(LPBYTE lpbKey,WORD wLen,LPKEYPH *lplpKeyPh)
{
	WORD wHead,wCount=0,wMask=0;
	LPKEYPH lpKPh;
	int i;
	BYTE abKey[MAX_PHRASE_LEN+1];
	BOOL fFirst;

	*lplpKeyPh = NULL;
	*(lplpKeyPh+1) = NULL;

	if(wLen<1) return 0;
	
	wHead = (WORD)(*(lpbKey+1));
	wHead |= ((WORD)(*lpbKey & 0x01)) << 8;
	wHead--;
	
	for(i=0;i<wLen;i++)
		wMask += 1<<i;

	for(i=0;i<2;i++){
		if( !(lpKPh=(LPKEYPH)((LPBYTE)lpMapFileBase+i*MAX_PY_NUM*sizeof(KEYPH))+wHead)->wLen ) {
			continue;
		}

		fFirst = TRUE;
		do {
			if(wCount && *(lplpKeyPh+i) != NULL) break;
			
			if( fFirst ) fFirst = FALSE;
			else lpKPh = lpKPh->lpNext;
			
			if( lpKPh->wLen >= wLen){
				memcpy(abKey,lpKPh->abKey,wLen+1);
				abKey[0] &= wMask;
				if(!memcmp(abKey,lpbKey,wLen+1)){
					if(lpKPh->wLen == wLen) *(lplpKeyPh+i) = lpKPh;
					else wCount++;
				}
			}
		}while(lpKPh->lpNext != NULL);
	}
	return wCount;
}

void InitDictionary()
{
	int i;
	BOOL fExist = FALSE;

	if ( (hMapFile=CreateFileMapping( INVALID_HANDLE_VALUE,
						NULL,
						PAGE_READWRITE ,
						0,
						MAPFILESIZE,
						MAPFILENAME)) == NULL) {
		MessageBox(NULL,"can not create filemapping","Init",MB_OK);
		exit(1);
	}

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		fExist = TRUE;
	}

	if ( (lpMapFileBase = (LPVOID) MapViewOfFile( hMapFile,
							FILE_MAP_ALL_ACCESS ,
							0,
							0,
							0)) == NULL) {
		MessageBox(NULL,"can not create filemapping","Init",MB_OK);
		exit(1);
	}

	if( !fExist || !dwMapFileOffset || !wMapCount){	
		for(i=0;i<2*MAX_PY_NUM*sizeof(KEYPH);i++){
			*((LPBYTE)lpMapFileBase+i)=0;
		}
		LoadPhrase();
	}

	LoadTable();

	wMapCount++;
}

void SortDic(LPHZPH *lplpPh, WORD wLen)
{
	int i,j;
	LPHZPH lpTempPh;
	
	for(i=0;i<wLen;i++){
		for(j=i;j<wLen;j++){
			if( (*(lplpPh+i))->dwAttrib < (*(lplpPh+j))->dwAttrib){
				lpTempPh = *(lplpPh+i);
				*(lplpPh+i)=*(lplpPh+j);
				*(lplpPh+j)=lpTempPh;
			}
		}
	}
}

void DestroyDictionary()
{
	int i,j,k;
	BYTE abKey[MAX_PHRASE_LEN+1];
	WORD wLen;
	BYTE bLen;
	LPKEYPH lpKPh;
	LPHZPH lpHZPh;
	FILE *stream;
	TCHAR szPhraseFileName[200],szTemp[200];
	LPTSTR lpPhraseFileName;
	BOOL fFirst1,fFirst2;
	LPHZPH alpHZPh[500];
	WORD wPhLen;

	for(i=0;i<MAX_PY_NUM;i++){
		if(alpHZTab[i] != NULL) free(alpHZTab[i]);
	}

	wMapCount--;
//	if( wMapCount > 0 ) goto my_exit;

	for(i=0;i<2;i++){
		if(!i){
			lpPhraseFileName = szPhraseFileName;
			lpPhraseFileName += GetSystemDirectory(szPhraseFileName,200);
			if (*(lpPhraseFileName-1) != _T('\\'))
				*lpPhraseFileName++ = _T('\\');
			_tcscpy(lpPhraseFileName,SYSPHRASEFILENAME);

			if( (stream = _tfopen( szPhraseFileName, _T("wb") )) == NULL ){
				_stprintf(szTemp,"%s can not found",szPhraseFileName);
				MessageBox(NULL,szTemp,"DicInit",MB_OK);
				continue;
			}
		}
		else{
			lpPhraseFileName = szPhraseFileName;
			lpPhraseFileName += GetSystemDirectory(szPhraseFileName,200);
			if (*(lpPhraseFileName-1) != _T('\\'))
				*lpPhraseFileName++ = _T('\\');
			_tcscpy(lpPhraseFileName,USRPHRASEFILENAME);

			if( (stream = _tfopen( szPhraseFileName, _T("wb") )) == NULL ){
				_stprintf(szTemp,"%s can not found",szPhraseFileName);
				MessageBox(NULL,szTemp,"DicInit",MB_OK);
				continue;
			}
		}
		for(j=0;j<MAX_PY_NUM;j++){
			if( !(lpKPh=(LPKEYPH)((LPBYTE)lpMapFileBase+i*MAX_PY_NUM*sizeof(KEYPH))+j)->wLen ) {
				continue;
			}
			fFirst1 = TRUE;
			do {
				if( fFirst1 ) fFirst1 = FALSE;
				else lpKPh = lpKPh->lpNext;

				if( lpKPh->wLen < 0 ) continue;
				wLen = lpKPh->wLen;
				bLen = (BYTE)wLen;
				lpHZPh = lpKPh->lpHZPH;
				memcpy(abKey,lpKPh->abKey,wLen+1);
				fFirst2 = TRUE;
				wPhLen = 0;
				do {
					if( fFirst2 ) fFirst2 = FALSE;
					else lpHZPh = lpHZPh->lpNext;
					alpHZPh[wPhLen++] = lpHZPh;
				}while(lpHZPh->lpNext != NULL);

				if( wConversionSet & CONVERSION_SET_SORT)
					SortDic(alpHZPh,wPhLen);

				for(k = 0;k<wPhLen;k++){
					fwrite(&bLen,1,1,stream);
					fwrite(abKey,1,wLen+1,stream);
					fwrite(alpHZPh[k]->szHZ,1,wLen*2,stream);
				}
			} while(lpKPh->lpNext != NULL);
		}
		fclose(stream);
	}
//my_exit:	
	UnmapViewOfFile(lpMapFileBase);
	CloseHandle(hMapFile);
}
