#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define CONVERSION_SET 0x6101
#define LAYOUTTEXT    _T("自由拼音输入法")
#define LAYOUTFILE    _T("kbdus.kbd")
#define LAYOUTFILENT  _T("kbdus.dll")
#define IMEFILE       _T("freepy.ime")

#define MAX_SIZE 200
#define MAX_SUBKEY 250

void main()
{
	HKEY hKey,hChildKey;
	DWORD dwIndex;
	DWORD dwName,dwMaxName = 0;
	TCHAR szNameArr[MAX_SUBKEY][MAX_SIZE],szName[MAX_SIZE];
	DWORD dwLen;
	TCHAR szLayoutText[MAX_SIZE];
	DWORD dwData;
	FILETIME ft;
	LONG lRet;
	TCHAR szRegName[MAX_SIZE];
	DWORD dwDisposition;
	DWORD dwConversionSet;
	WORD wCount = 0;
	int i;
	OSVERSIONINFO osVerInfo;

	dwIndex = 0;
	dwLen = MAX_SIZE;
	dwData = MAX_SIZE * sizeof(TCHAR);
	dwConversionSet = CONVERSION_SET;

	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			_T("Software"),
			0,
			KEY_READ,
			&hKey) != ERROR_SUCCESS ){
		_ftprintf(stderr,_T("Software open error\n"));
		exit(1);
	}

	if( RegCreateKeyEx( hKey,
					_T("freepy"),
					0,
					NULL,
					REG_OPTION_NON_VOLATILE ,
					KEY_ALL_ACCESS ,
					NULL,
					&hChildKey,
					&dwDisposition ) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		_ftprintf(stderr,_T("freepy create error\n"));
		exit(1);
	}

	RegSetValueEx(hChildKey,
				_T("ConversionSet"),
				0,
				REG_DWORD,
				(LPBYTE)&dwConversionSet,
				sizeof(DWORD));

	RegCloseKey(hChildKey);
	RegCloseKey(hKey);

	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			_T("System\\CurrentControlSet\\Control\\Keyboard Layouts"),
			0,
			KEY_READ,
			&hKey) != ERROR_SUCCESS ){
		_ftprintf(stderr,_T("Keyboard Layouts open error\n"));
		exit(1);
	}

	RegQueryInfoKey (hKey,NULL,	NULL,NULL,
					&dwIndex,
					NULL,NULL,NULL,	NULL,NULL,NULL,	NULL);

	while( (int)dwIndex >= 0 ) {
		dwLen = MAX_SIZE;
		if((lRet = RegEnumKeyEx(hKey,
					dwIndex,
					szName,
					&dwLen,
					NULL,
					NULL,
					NULL,
					&ft)) == ERROR_SUCCESS || lRet == ERROR_MORE_DATA ) {
			_tcscpy(szNameArr[wCount++],szName);
		}
		dwIndex--;
	}

	for(i=0;i<wCount;i++) {
		if ( RegOpenKeyEx(hKey,
			szNameArr[i],
			0,
			KEY_ALL_ACCESS ,
			&hChildKey) == ERROR_SUCCESS ){
			if( RegQueryValueEx( hChildKey,
								_T("Layout Text"),
								NULL,
								NULL,
								(LPBYTE)szLayoutText,
								&dwData) == ERROR_SUCCESS ){
				if(!_tcsicmp(szLayoutText,LAYOUTTEXT)) {
					goto my_next;
				}
				_stscanf(szNameArr[i],_T("%lx"),&dwName);
				if( dwMaxName < dwName ) dwMaxName = dwName;
			}
			RegCloseKey(hChildKey);
		}
	}

	_stprintf(szRegName,"%lx",dwMaxName + 0x10000);
	CharUpper(szRegName);

	if( RegCreateKeyEx( hKey,
					szRegName,
					0,
					NULL,
					REG_OPTION_NON_VOLATILE ,
					KEY_ALL_ACCESS ,
					NULL,
					&hChildKey,
					&dwDisposition ) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		_ftprintf(stderr,_T("%s create error\n"),szRegName);
		exit(1);
	}

my_next:
	RegSetValueEx(hChildKey,
				_T("IME File"),
				0,
				REG_SZ,
				(LPBYTE)IMEFILE,
				_tcslen(IMEFILE)*sizeof(TCHAR));

	osVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&osVerInfo)) {
		RegSetValueEx(hChildKey,
				_T("Layout File"),
				0,
				REG_SZ,
				(osVerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) ?
					((LPBYTE)LAYOUTFILENT):((LPBYTE)LAYOUTFILE),
				_tcslen(LAYOUTFILENT)*sizeof(TCHAR));
	}
	else {
		_ftprintf(stdout,_T("get version error\n"));
		RegSetValueEx(hChildKey,
				_T("Layout File"),
				0,
				REG_SZ,
				(LPBYTE)LAYOUTFILE,
				_tcslen(LAYOUTFILE)*sizeof(TCHAR));
	}

	RegSetValueEx(hChildKey,
				_T("Layout Text"),
				0,
				REG_SZ,
				(LPBYTE)LAYOUTTEXT,
				_tcslen(LAYOUTTEXT)*sizeof(TCHAR));

	RegCloseKey(hChildKey);
	RegCloseKey(hKey);

	_ftprintf(stdout,_T("ok\n"));
	return;
}
