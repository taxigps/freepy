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
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#define _NO_EXTERN_
#include "HZfreepy.h"
#include "freepy.h"

/* For debug ****************************************/
int nDebugLevel = 8;
FILE *DebugLogFile = NULL;
/****************************************************/

LPVOID         lpMapFileBase = NULL;
HANDLE         hMapFile;

HINSTANCE      hInst;
HKL            hFreePYKL = 0;

LPDWORD        lpdwCurTransKey = NULL;
UINT           uNumTransKey;
BOOL           fOverTransKey = FALSE;

WORD           wConversionMode = 0;
SIZE           sizeCand[2];

PINYIN      aPYTab[26][MAX_EACH_PY_NUM] = {0};
TCHAR       aPunct[_T('~') - _T('!') +1][MAXCANDSTRNUM][MAXCANDSTRSIZE] = {0};
LPTSTR      alpHZTab[MAX_PY_NUM] = {0};

#pragma data_seg(".freepydic")
DWORD       dwMapFileOffset = 0;
DWORD       dwMapFileUsrOffset = 0;
WORD        wMapCount = 0;
WORD        wConversionSet = 0;
#pragma data_seg()

