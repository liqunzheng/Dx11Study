#include "stdafx.h"
#include "D3D11Class.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "ShapeManager.h"
#include "FontTextureManager.h"


CD3D11Class::CD3D11Class()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthDisabledStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;


	m_shaderMgr = new CShaderManager();
	m_textureMgr = new CTextureManager();
	m_shapeMgr = new CShapeManager();
	m_fontTexMgr = new CFontTextureManager();

	m_alphaEnableBlendingState = 0;
	m_alphaDisableBlendingState = 0;
}


CD3D11Class::~CD3D11Class()
{
	Shutdown();
}

bool CD3D11Class::Initialize(bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	int screenWidth, screenHeight;
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		screenWidth = max(1, rect.right - rect.left);
		screenHeight = max(1, rect.bottom - rect.top);
	}

	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator = 0, denominator = 1;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// ���洹ֱͬ������
	m_vsync_enabled = vsync;

	// ����һ��DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// �ýӿڹ�������һ�����Կ�������
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// �õ��������������.
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	//�õ��ʺ� DXGI_FORMAT_R8G8B8A8_UNORM ����ʾģʽ.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// ������ʾģʽ��displayModeList��
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	//����������ʾģʽ���õ�ˢ������������ֵnumerator �� denominato
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	// �õ��Կ�����
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// �����Դ��С.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//�����Կ�������
	m_strCardDescription = adapterDesc.Description;

	// �ͷ���ʾģʽ�б�
	delete[] displayModeList;
	displayModeList = 0;

	//�ͷ����������.
	adapterOutput->Release();
	adapterOutput = 0;

	//�ͷ�������
	adapter->Release();
	adapter = 0;

	// �ͷŽӿڹ���.
	factory->Release();
	factory = 0;

	// ��ʼ������������
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// ��1���󻺳�
	swapChainDesc.BufferCount = 1;

	//֡����Ĵ�С��Ӧ�ó��򴰿ڴ�С���.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// �󻺳��surface�ĸ�ʽΪDXGI_FORMAT_R8G8B8A8_UNORM.
	// surface��ÿ��������4���޷��ŵ�8bit[ӳ�䵽0-1]����ʾ��NORM��ʾ��һ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// ���ʹ�ô�ֱͬ�������ú󻺳��ˢ���ʡ�.
	//ˢ���ʾ���һ���ӰѺ󻺳���������Ļ�ϻ����Ĵ�����
	//���������ֱͬ����������ˢ���ʣ���fps�ǹ̶���
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// ���ú󻺳����;
	// ���ǵ���ȾĿ�껺��Ϊ�󻺳塣
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// �󻺳�����Ĵ��ھ��.
	swapChainDesc.OutputWindow = hwnd;

	// ��ʹ�ö��ز���
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// ����ȫ�����ߴ���ģʽ.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// �趨ɨ����ordering�Լ�����Ϊunspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// �󻺳����ݳ��ֵ���Ļ�󣬷���������
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//�����ñ�־
	swapChainDesc.Flags = 0;

	// ����feature levelΪD3D11
	// ����Կ���֧��D3D11,�����ܹ�ͨ���������������ʹ��D3D10,����9.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// �������������豸�Լ��豸������.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	// �õ��������еĺ󻺳�ָ��.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// �ú󻺳崴����ȾĿ����ͼ.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	//�ͷź󻺳�.(���ü�����1)
	backBufferPtr->Release();
	backBufferPtr = 0;

	// ��ʼ����Ȼ�������.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	//������Ȼ�������
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;//������Ȼ���Ϊ1
	depthBufferDesc.ArraySize = 1;//������Ȼ���Ϊ1������������2�������и�����;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// ������Ȼ���.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// ��ʼ�����ģ��״̬����.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// �������ģ��״̬����.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;//D3D11_DEPTH_WRITE_MASK_ZERO��ֹд��Ȼ���
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// ����front face ����ʹ�õ�ģ���������.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// ����back face����ʹ�õ�ģ�����ģʽ.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �������ģ��״̬��ʹ����Ч
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;

	}

	// �������ģ��״̬.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// ��ʼ�����ģ����ͼ.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// �������ģ����ͼ����.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// �������ģ����ͼ.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// ����ȾĿ����ͼ����Ȼ��嵽��Ⱦ����.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);


	// ���ù�դ��������ָ���������α���Ⱦ.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// ������դ��״̬
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	//���ù�դ��״̬��ʹ����Ч
	m_deviceContext->RSSetState(m_rasterState);


	// �����ӿڣ���ʾȫ���󻺳�����
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// �����ӿ�
	m_deviceContext->RSSetViewports(1, &viewport);

	// ����͸��ͶӰ����
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// ����͸��ͶӰ����.
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	// ��������ͶӰ������Ҫ����ʵʩ2D��Ⱦ.
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);


	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// ��ʼ��blend������
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// ����һ��alpha blend״̬.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//0x0f;

	// ������������һ��alpha blend״̬
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	//�޸�������.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	//����һ���µ�blend״̬.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	//��ʼ��ģ��
	m_fontTexMgr->Initialize(this);
	m_shapeMgr->Initialize(L"", this);

	return true;
}

void CD3D11Class::Shutdown()
{
	if (m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = 0;
	}

	if (m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = 0;
	}

	if (m_shaderMgr)
	{
		m_shaderMgr->Shutdown();
		delete m_shaderMgr;
		m_shaderMgr = 0;
	}

	if (m_textureMgr)
	{
		m_textureMgr->Shutdown();
		delete m_textureMgr;
		m_textureMgr = 0;
	}

	if (m_shapeMgr)
	{
		m_shapeMgr->Shutdown();
		delete m_shapeMgr;
		m_shapeMgr = 0;
	}

	if (m_fontTexMgr)
	{
		m_fontTexMgr->Shutdown();
		delete m_fontTexMgr;
		m_fontTexMgr = nullptr;
	}

	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}
}

void CD3D11Class::BeginScene(float red, float green, float blue, float alpha)
{
	// ��������󻺳���ɫ.
	float color[] = {red, green, blue, alpha};

	//����󻺳�.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	//�����Ȼ���.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CD3D11Class::EndScene()
{
	//��Ⱦ��ɺ󣬰Ѻ󻺳�����present��ǰ����
	if (m_vsync_enabled)
	{
		// ������Ļˢ����.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// �����ܿ��present�󻺳�.
		m_swapChain->Present(0, 0);
	}
}

ID3D11Device* CD3D11Class::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* CD3D11Class::GetDeviceContext()
{
	return m_deviceContext;
}

const GeMatrix& CD3D11Class::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

const GeMatrix& CD3D11Class::GetOrthoMatrix() const
{
	return m_orthoMatrix;
}

const std::wstring& CD3D11Class::GetVideoCardInfo(int& memory) const
{
	memory = m_videoCardMemory;
	return m_strCardDescription;
}

CShaderManager* CD3D11Class::getShaderMgr()
{
	return m_shaderMgr;
}

CTextureManager* CD3D11Class::getTextureMgr()
{
	return m_textureMgr;
}

CShapeManager *CD3D11Class::getModelMgr()
{
	return m_shapeMgr;
}

void CD3D11Class::Render()
{
	m_shapeMgr->Render(this);
}

GeMatrix CD3D11Class::getViewMatrix()
{
	GeMatrix mx;
	m_camera.getViewMatrix(&mx);
	return mx;
}

void CD3D11Class::TurnZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
}

void CD3D11Class::TrueZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}

CCamera* CD3D11Class::getCamera()
{
	return &m_camera;
}

void CD3D11Class::TurnOnAlphaBlending()
{
	float blendFactor[4];

	// ����blend����
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// ��alpha blend
	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void CD3D11Class::TurnOffAlphaBlending()
{
	float blendFactor[4];

	// ����blend����
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// �ر�alpha blend
	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}

void CD3D11Class::getViewSize(int& iWidth, int& iHeight)
{
	D3D11_VIEWPORT viewport;
	UINT uNum = 1;
	m_deviceContext->RSGetViewports(&uNum, &viewport);
	iWidth = static_cast<int>(viewport.Width);
	iHeight = static_cast<int>(viewport.Height);
}

CFontTextureManager* CD3D11Class::getFontMgr()
{
	return m_fontTexMgr;
}

