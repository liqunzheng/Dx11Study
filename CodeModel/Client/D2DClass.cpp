#include "D2DClass.h"


D2DClass::D2DClass()
{
	m_pRenderTarget = nullptr;
	m_pText = nullptr;
	m_pBlackBrush = nullptr;
}


D2DClass::~D2DClass()
{
}

bool D2DClass::InitD2D(HWND hwnd)
{
	// 全局变量:
	ID2D1Factory*          pD2DFactory = NULL;
	IDWriteFactory       * p_pDWriteFactory = NULL;


	//创建一个Direct2D资源指针
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite factory.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(p_pDWriteFactory),
			reinterpret_cast<IUnknown **>(&p_pDWriteFactory)
			);
	}

	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = p_pDWriteFactory->CreateTextFormat(
			L"微软雅黑",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			12,
			L"", //locale
			&m_pText
			);
	}

	if (SUCCEEDED(hr))
	{
		// Center the text horizontally and vertically.
		m_pText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

		m_pText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	}
	// Obtain the size of the drawing area.
	RECT rc;
	GetClientRect(hwnd, &rc);

	// Create a Direct2D render target			
	hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&m_pRenderTarget);
	// 设置笔刷
	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&m_pBlackBrush
			);
	}
	if (SUCCEEDED(hr))
		return true;

	return false;
}

void D2DClass::Render()
{
	static const WCHAR sc_helloWorld[] = L"看不懂中文吧！笨蛋！";
	if (!m_pRenderTarget)
		return;
	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	m_pRenderTarget->DrawText(
		sc_helloWorld,
		ARRAYSIZE(sc_helloWorld) - 1,
		m_pText,
		D2D1::RectF(0, 0, 800, 800),
		m_pBlackBrush
		);
	m_pRenderTarget->EndDraw();
}
