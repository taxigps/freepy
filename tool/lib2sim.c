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

int lib2sim(LPTSTR lpInName,LPTSTR lpOutName)
{
	FILE *out,*stream;
	LPPINYIN lpPYTab = (LPPINYIN)aPYTab;
	BYTE abKey[MAX_PHRASE_LEN+2];
	TCHAR szStr[2*MAX_PHRASE_LEN+2];
	BYTE bLen;
	WORD wLen,wKey,wTempKey;

	int i,j,k;
	
	if( (stream = _tfopen( lpInName, _T("rb") )) == NULL ){
		fprintf(stderr,_T("%s can not found"),lpInName);
		exit(1);
	}
	if( (out = _tfopen( lpOutName, _T("w") )) == NULL ){
		fprintf(stderr,"%s can not found",lpOutName);
		exit(1);
	}

	while( !feof( stream )) {
		if(fread(&bLen,1,1,stream)){
			wLen=(WORD)bLen;
			if( wLen > 0 && fread(abKey,1,wLen+1,stream) &&
				fread(szStr,1,wLen*2,stream) ) {
				szStr[wLen*2] = _T('\0');
				fprintf(out,"%s\t",szStr);
				for(k=0;k<wLen;k++){
					wKey = (WORD)abKey[0];
					wKey = (wKey << (8-k)) & 0x0100;
					wKey += (WORD)abKey[k+1];
					for(i=0;i<26;i++){
						for(j=0;(wTempKey = (lpPYTab+i*MAX_EACH_PY_NUM+j)->wKey);j++){
							if(wKey == wTempKey){
								fprintf(out,"%s",(lpPYTab+i*MAX_EACH_PY_NUM+j)->szPY);
								if(k!=wLen-1)
									fprintf(out," ");
								else
									fprintf(out,"\n");
							}
						}
					}
				}
			}
			else goto my_exit;
		}
		else{
			goto my_exit;
		}
	}
my_exit:
	fclose(stream);
	fclose(out);
	return 1;
}	

void main(int argc, char **argv)
{
	if(argc != 3) {
		fprintf(stderr,"usage: %s <input_name> <output_name>\n",argv[0]);
		return;
	}
	LoadTable();
	lib2sim(argv[1],argv[2]);
	return;
}
