#include "GraphicsClass.h"
#include "D3D11Class.h"
#include "Camera.h"

//namespace
//{
//	void transform_ray(CRay* ray, D3DXMATRIX* trans_matrix)
//	{
//		// transform the ray's origin, w = 1.
//		D3DXVec3TransformCoord(&ray->m_ptOrigin, &ray->m_ptOrigin, trans_matrix);
//
//		// transform the ray's direction, w = 0.
//		D3DXVec3TransformNormal(&ray->m_ptDirection, &ray->m_ptDirection, trans_matrix);
//
//		// normalize the direction
//		D3DXVec3Normalize(&ray->m_ptDirection, &ray->m_ptDirection);
//	}
//}


GraphicsClass::GraphicsClass(void)
{
	m_p3dRoot = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass&)
{

}
GraphicsClass::~GraphicsClass(void)
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
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

void GraphicsClass::Shutdown()
{
	if (m_p3dRoot)
	{
		m_p3dRoot->Shutdown();
		delete m_p3dRoot;
		m_p3dRoot = 0;
	}
	return;
}


bool GraphicsClass::Frame()
{
	D3DXMATRIX mxView;
	//m_Camera->getViewMatrix(&mxView);
	//m_p3dRoot->setViewMatrix(mxView);
	m_p3dRoot->BeginScene(0.0f, 0.0f, 0.5f, 1.0f);

	m_p3dRoot->Render();
	m_p3dRoot->EndScene();

	return true;
}

void GraphicsClass::pick(int x, int y)
{
	//CRay ray = m_D3D->calculate_picking_ray(x, y);
	//D3DXMATRIX view_matrix, view_inverse_matrix;
	//m_Camera->getViewMatrix(&view_matrix);

	//D3DXMatrixInverse(&view_inverse_matrix, NULL, &view_matrix);

	//transform_ray(&ray, &view_inverse_matrix);

	//bool b = m_CubeModel->InsectRay(&ray);

	//if (b)
	//{
	//	DPrintf("Insection ...");
	//}
}

CCamera* GraphicsClass::getCamera()
{
	if (m_p3dRoot != nullptr)
	{
		return m_p3dRoot->getCamera();
	}
}
