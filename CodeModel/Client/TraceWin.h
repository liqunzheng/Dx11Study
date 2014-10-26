#pragma once


#include <windows.h>
#include <TCHAR.h>

#define ID_COPYDATA_TRACEMSG MAKELONG(MAKEWORD('t','w'),MAKEWORD('i','n'))

class CDebugPrint
{
public:
	static void debugPrint(const char* lpBuf)
	{
		HWND hTraceWnd = FindWindow(_T("MfxTraceWindow"), NULL);
		DWORD nCount = strlen(lpBuf);

		if (hTraceWnd == NULL || nCount >= 1023)
		{
			return;
		}
		static char mybuf[1024];
		memcpy(mybuf, lpBuf, nCount);
		mybuf[nCount] = '\n';

		COPYDATASTRUCT cds;
		cds.dwData = ID_COPYDATA_TRACEMSG;
		cds.cbData = nCount + 1;
		cds.lpData = mybuf;
		SendMessage(hTraceWnd, WM_COPYDATA, NULL, (LPARAM)&cds);
	}
};

#define DPrintf(msg) CDebugPrint::debugPrint(msg)


