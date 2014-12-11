#include "stdafx.h"
#include "D3DManager.h"
#include "D3D11Class.h"
#include "Camera.h"

static const bool FULL_SCREEN = false;//是否全屏
static const bool VSYNC_ENABLED = true;//是否垂直同步
static const float SCREEN_DEPTH = 1000.0f;//深度，远点
static const float SCREEN_NEAR = 0.1f;//深度，近点


CD3DManager::CD3DManager()
{
	m_p3dRoot = nullptr;
}

CD3DManager::~CD3DManager()
{
}

CD3DManager* CD3DManager::Instance()
{
	static CD3DManager myself;
	return &myself;
}

bool CD3DManager::InitEnger(HWND hwnd)
{
	//如果对象已经存在，先释放掉它们
	Shutdown();

	m_p3dRoot = new CD3D11Class();
	if (!m_p3dRoot)
	{
		return false;
	}

	bool bRet = m_p3dRoot->Initialize(VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!bRet)
	{
		return false;
	}

	//创建摄像机对象

	// 设置摄像机位置.
	D3DXVECTOR3 campos = D3DXVECTOR3(0.0f, 0.0f, -10.0f);
	m_p3dRoot->getCamera()->setPosition(&campos);
	return true;
}

void CD3DManager::Render()
{
	m_p3dRoot->BeginScene(0.0f, 0.0f, 0.5f, 1.0f);
	m_p3dRoot->Render();
	m_p3dRoot->EndScene();
}

void CD3DManager::Shutdown()
{
	if (m_p3dRoot)
	{
		m_p3dRoot->Shutdown();
		delete m_p3dRoot;
		m_p3dRoot = nullptr;
	}
}

CCamera* CD3DManager::getCamera()
{
	ASSERT(m_p3dRoot);
	return m_p3dRoot->getCamera();
}

