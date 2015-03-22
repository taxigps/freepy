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

#ifndef _HZFREEPY_H_
#define _HZFREEPY_H_

/*for debug*************************/
//#define _MY_DEBUG

#ifdef _MY_DEBUG
#define DEBUGLOGFILE  _T("d:\\work\\c\\freepy\\release\\debug.log")
#define DebugLog(n,f) (void)((DebugLogFile != NULL && nDebugLevel & n) ? (_ftprintf f,fflush(DebugLogFile)) : 0)
#else
#define DebugLog(n,f) (void)0
#endif //_MY_DEBUG
/***********************************/

#define TABFILENAME _T("freepy.tab")
#define SYSPHRASEFILENAME _T("freepysys.dic")
#define USRPHRASEFILENAME _T("freepyusr.dic")
#define MAPFILENAME _T("FreePYShareMemory")

#define MAPFILESIZE  (4096*1500) //6M

/* for limit of FREEPYIME */
#define MAXCOMPSIZE             80
#define MAXCANDSTRSIZE          32
#define MAXCANDSTRNUM           40


#define SELECT_FORWARD 0x01
#define SELECT_BACKWARD 0x02

#define CAND_PAGESIZE 10

#define MAX_PHRASE_LEN 8
#define MAX_PY_LEN 8
#define MAX_PY_NUM 420


#define MAX_EACH_PY_NUM 41

#define MAX_SELECT_PH 500

typedef struct {
	WORD wKey;
	TCHAR szPY[MAX_PY_LEN];
} PINYIN,FAR *LPPINYIN;

extern LPVOID  lpMapFileBase;
#ifndef _NO_EXTERN_
extern FILE *DebugLogFile;
extern int nDebugLevel;
extern PINYIN  aPYTab[][MAX_EACH_PY_NUM];
extern LPTSTR  alpHZTab[];
extern TCHAR   aPunct[][MAXCANDSTRNUM][MAXCANDSTRSIZE];
extern DWORD   dwMapFileOffset;
extern DWORD   dwMapFileUsrOffset;
extern WORD    wMapCount;
extern HANDLE  hMapFile;
#endif _NO_EXTERN_

typedef struct _HZPH {
	TCHAR szHZ[2*(MAX_PHRASE_LEN+1)];
	DWORD dwAttrib;
	struct _HZPH __based(lpMapFileBase) *lpNext;
} HZPH, FAR *LPHZPH;

typedef struct _KEYPH {
	SHORT wLen;
	BYTE abKey[MAX_PHRASE_LEN+1];
	HZPH __based(lpMapFileBase) *lpHZPH;
	struct _KEYPH __based(lpMapFileBase) *lpNext;
} KEYPH, FAR *LPKEYPH;


typedef struct {
	KEYPH *lpKeyPH;
	HZPH *lpHZPH;
} PHRASE, FAR *LPPHRASE;


typedef struct {
	LPTSTR lpHZ;
	WORD wKey;
	WORD wLen;
} HANZI, FAR *LPHANZI;

typedef struct {
	TCHAR szPaintCompStr[MAXCOMPSIZE];
	TCHAR szConvCompStr[MAXCOMPSIZE];
	TCHAR szPreResultStr[MAXCOMPSIZE];
	TCHAR szPYArray[2*MAX_PHRASE_LEN][MAXCOMPSIZE];
	SHORT wEditCaret;
	SHORT wUnConvPos;
	WORD wPYArrayLen;
	WORD wPYArrayCurPos;
} FREEPYCOMP, FAR *LPFREEPYCOMP;

typedef struct {
	
	HANZI aHanZi[MAX_EACH_PY_NUM];
	PHRASE aPhrase[MAX_SELECT_PH];
	WORD wPhraseNum; //means the number of phrase candadate.
	WORD wHZNum; // means the number of struct HANZI candadate.
	
	SHORT awBeforePos[2];
	SHORT awCurrentPos[2];
	WORD wSelectDirect;
	WORD wSelectStatus; // 0 means select hanzi, 1 means select phrase.
	
	BYTE abKey[MAX_PHRASE_LEN+1];
	WORD wKeyLen;
} FREEPYCAND, FAR *LPFREEPYCAND;

//HZmodenormal.c
BOOL CharHandleNormal( HIMC,WORD,LONG);
BOOL AddChar( HIMC, WORD, WORD);
BOOL ForwardPage(HIMC);
BOOL BackwardPage(HIMC);
BOOL SelectCand(HIMC,WORD);
BOOL DeletePhrase(HIMC,WORD);

//HZutil.c
WORD ParsePY(LPTSTR, LPTSTR, WORD);
void ConvertPY(LPTSTR,WORD,WORD,LPFREEPYCAND);
WORD CreateCandStr(LPFREEPYCAND , WORD , LPTSTR,WORD);
void CreatePaintStr(LPTSTR,WORD,WORD,LPTSTR,LPTSTR );
void SortPhrase(LPPHRASE, WORD);
WORD EffectPYArrayLen( LPTSTR,WORD,WORD);
WORD CalculatePosSpan(LPTSTR,WORD ,WORD,WORD);
SHORT CalculateUnConvPos(LPTSTR,WORD ,WORD,LPTSTR);

//HZlib.c
void InitDictionary();
void DestroyDictionary();
void SavePhToMapFile(LPTSTR,LPBYTE,WORD,WORD);
WORD QueryPhrase(LPBYTE ,WORD,LPKEYPH *);

//HZspecial.c
BOOL RepeatPreResult(HIMC );
BOOL GeneratePunct(HIMC, WORD);

//HZmodephtoch.c
BOOL CharHandlePhToCh( HIMC,WORD,LONG);

//HZmodei.c
BOOL CharHandleI( HIMC,WORD,LONG);

//HZmodeu.c
BOOL CharHandleU( HIMC,WORD,LONG);

//HZmodev.c
BOOL CharHandleV( HIMC,WORD,LONG);

#endif /* _HZFREEPY_H_ */
