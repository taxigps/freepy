#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <windows.h>
#include <tchar.h>

#define MAX_PHRASE_LEN 8
#define MAX_PY_NUM 420
#define MAX_EACH_PY_NUM 41

typedef struct {
	WORD wKey;
	TCHAR szPY[8];
} PINYIN,FAR *LPPINYIN;

PINYIN      aPYTab[26][MAX_EACH_PY_NUM] = {0};

void LoadHZDictionary( LPTSTR lpStr)
{
	TCHAR szPY[20],szHZ[1000];
	static int i=0,j=0,nPre=0;
	WORD wPYHead=1;
	LPPINYIN lpPYTab = (LPPINYIN)aPYTab;

	_stscanf(lpStr,"%s %s",szPY,szHZ);

	
	wPYHead=(WORD)szPY[0] - (WORD)_T('a');
	if(wPYHead != nPre) j=0;
	_tcscpy( (lpPYTab+wPYHead*MAX_EACH_PY_NUM+j)->szPY,szPY);
	(lpPYTab+wPYHead*MAX_EACH_PY_NUM+j)->wKey=i+1;

	nPre=wPYHead;
	i++,j++;
	return;
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
	TCHAR szStr[1000];
	TCHAR szTabFileName[200];
	LPTSTR lpTabFileName = szTabFileName;


	lpTabFileName += GetSystemDirectory(szTabFileName,200);
	if (*(lpTabFileName-1) != _T('\\'))
		*lpTabFileName++ = _T('\\');
	_tcscpy(lpTabFileName,_T("freepy.tab"));

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
					//LoadPunct( szStr );
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

int sim2lib(LPTSTR lpInName,LPTSTR lpOutName)
{
	FILE *stream,*out;
	int i,j;
	TCHAR szStr[250];
	WORD wLen,wHead,flag;
	BYTE abKey[MAX_PHRASE_LEN+2],bLen;
	WORD awKey[MAX_PHRASE_LEN];
	WORD wCount;
	TCHAR szStrArr[MAX_PHRASE_LEN+4][2*MAX_PHRASE_LEN+2];
	
	if( (stream = _tfopen( lpInName, _T("r") )) == NULL ){
		fprintf(stderr,"%s cant open.\n",lpInName);
		exit(1);
	}
	
	if( (out = _tfopen( lpOutName, _T("wb") )) == NULL ){
		fprintf(stderr,"%s cant open.\n",lpOutName);
		exit(1);
	}

	while( !feof( stream )) {
		if( _fgetts(szStr,1000,stream) != NULL){
			*(szStr+_tcslen(szStr)-1)=_T('\0');
			wCount=String2Array(szStr,(LPTSTR)szStrArr,2*MAX_PHRASE_LEN+2);
			wLen=_tcslen(szStrArr[0])/2;
			if(wLen != wCount-1 || wLen > MAX_PHRASE_LEN){
				printf("%s\n",szStr);
				continue;
			}
			for(i=1;i<wCount;i++){
				wHead=(int)szStrArr[i][0] - _T('a');
				flag=1;
				if(wHead < 0 || wHead > 25){
					printf("%s\n",szStr);
					flag=0;
					break;
				}
				flag=0;
				for(j=0;aPYTab[wHead][j].wKey;j++){
					if(!_tcscmp(aPYTab[wHead][j].szPY,szStrArr[i])){
						awKey[i-1]=aPYTab[wHead][j].wKey;
						flag=1;
						break;
					}
				}
				if(!flag) break;
			}
			if(!flag){
				printf("%s\n",szStr);
				continue;
			}
			for(i=0;i<wLen;i++)
				abKey[i+1] = awKey[i] & 0xff;
			
			abKey[0]=_T('\0');
			for(i=0;i<wLen;i++)
				abKey[0] |= (awKey[i] & 0x0100) >> (8-i);
			
			bLen=(BYTE)wLen;
			
			fwrite(&bLen,1,1,out);
			fwrite(abKey,1,wLen+1,out);
			fwrite(szStr,1,wLen*2,out);
		}
	}
	
	fclose(stream);
	fclose(out);
	return (0);
}

void main(int argc,char **argv)
{
  if(argc != 3) {
    fprintf(stderr,"usage: %s <input_name> <output_name>\n",argv[0]);
    return;
  }
  LoadTable();
  sim2lib(argv[1],argv[2]);
  return;
}
