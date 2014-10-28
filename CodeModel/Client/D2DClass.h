#pragma once
#include <Windows.h>
#include <tchar.h>
#include <D2D1.h>
#include <DWrite.h>
#include <wincodec.h>

class D2DClass
{
public:
	D2DClass();
	~D2DClass();
	bool InitD2D(HWND hwnd);
	void Render();

private:
	ID2D1HwndRenderTarget	*m_pRenderTarget;
	IDWriteTextFormat		*m_pText;
	ID2D1SolidColorBrush	*m_pBlackBrush = NULL;     // ÊµÀý±ÊË¢

};

