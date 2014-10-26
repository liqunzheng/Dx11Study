#include <windows.h>
#include <TCHAR.h>

#define ID_COPYDATA_TRACEMSG MAKELONG(MAKEWORD('t','w'),MAKEWORD('i','n'))

void DPrintf(const char* lpBuf)
{
	//#ifdef _UNICODE
	//		BOOL bDefCharUsed;
	//		::WideCharToMultiByte(CP_ACP,0,LPCWSTR(lpBuf),
	//			-1, mybuf, 1024, NULL, &bDefCharUsed);
	//#else
	//		memcpy(mybuf, lpBuf, nCount);
	//#endif
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
	cds.cbData = nCount+1;
	cds.lpData = mybuf;
	SendMessage(hTraceWnd, WM_COPYDATA, NULL, (LPARAM)&cds);
}

