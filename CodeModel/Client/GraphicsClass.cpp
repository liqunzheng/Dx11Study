#include "GraphicsClass.h"

namespace
{
	void transform_ray(sRay* ray, D3DXMATRIX* trans_matrix)
	{
		// transform the ray's origin, w = 1.
		D3DXVec3TransformCoord(&ray->origin, &ray->origin, trans_matrix);

		// transform the ray's direction, w = 0.
		D3DXVec3TransformNormal(&ray->direction, &ray->direction, trans_matrix);

		// normalize the direction
		D3DXVec3Normalize(&ray->direction, &ray->direction);
	}
}


GraphicsClass::GraphicsClass(void)
{
	m_D3D = 0;
	m_D2D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_AxisModel = 0;
	m_ColorShader = 0;

	m_PlaneModel = 0;
	m_CubeModel = 0;
	m_Light = 0;
	m_LightShader = 0;

}

GraphicsClass::GraphicsClass(const GraphicsClass&)
{

}
GraphicsClass::~GraphicsClass(void)
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	//如果对象已经存在，先释放掉它们
	Shutdown();

	// 创建一个D3DClass对象.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// 调用D3DClass初始化函数
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_D2D = new D2DClass;
	if (!m_D2D)
	{
		return false;
	}
	result = m_D2D->InitD2D(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct2D", L"Error", MB_OK);
		return false;
	}

	//创建摄像机对象
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// 设置摄像机位置.
	D3DXVECTOR3 campos = D3DXVECTOR3(8.0f, 3.0f, -20.0f);
	m_Camera->setPosition(&campos);

	// 创建模型对象.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// 初始化模型对象.
	result = m_Model->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// 创轴建模型对象.
	m_AxisModel = new AxisModelClass;
	if (!m_AxisModel)
	{
		return false;
	}
	// 初始化坐标轴模型对象.
	result = m_AxisModel->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the axis model object.", L"Error", MB_OK);
		return false;
	}

	// 创平面模型对象.
	m_PlaneModel = new PlaneModelClass;
	if (!m_PlaneModel)
	{
		return false;
	}
	// 初始化平面模型对象.
	result = m_PlaneModel->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the plane model object.", L"Error", MB_OK);
		return false;
	}

	// 创建cube模型对象
	m_CubeModel = new CubeModelClass;
	if (!m_CubeModel)
	{
		return false;
	}
	// 初始化坐标cube模型对象.
	result = m_CubeModel->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the axis model object.", L"Error", MB_OK);
		return false;
	}

	// 创建shader对象
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// 初始化shader对象
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// 创建光照shader类
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	//初始化光照shader对象
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// 创建光源对象.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{

	// 释放光源对象.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// 释放光照材质shader对象.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// 释放shader对象.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// 释放模型对象.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
	// 释放坐标轴模型对象.
	if (m_AxisModel)
	{
		m_AxisModel->Shutdown();
		delete m_AxisModel;
		m_AxisModel = 0;
	}

	//释放plane模型对象
	if (m_PlaneModel)
	{
		m_PlaneModel->Shutdown();
		delete m_PlaneModel;
		m_PlaneModel = 0;
	}

	//释放cube模型对象
	if (m_CubeModel)
	{
		m_CubeModel->Shutdown();
		delete m_CubeModel;
		m_CubeModel = 0;
	}

	// 释放摄像机对象
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	//销毁m_D3D对象
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;


	// 调用Render函数，渲染3D场景
	// Render是GraphicsClass的私有函数.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	m_D2D->Render();
	return true;



	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;


	// 设置framebuffer.为浅蓝色
	m_D3D->BeginScene(0.0f, 0.0f, 0.5f, 0.5f);

	// 得到3个矩阵.
	m_Camera->getViewMatrix(&viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_AxisModel->Render(m_D3D->GetDeviceContext());
	// 用shader渲染.
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_AxisModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	// 把模型顶点和索引缓冲放入管线，准备渲染.
	m_Model->Render(m_D3D->GetDeviceContext());

	// 用shader渲染.
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}


	//得到摄像机位置和一些材质光照系数
	D3DXVECTOR3 camerapos;
	D3DXVECTOR4 realcamerpos;
	D3DXVECTOR4 Ke = D3DXVECTOR4(0.8, 0.0, 0.2, 1.0);
	D3DXVECTOR4 Ka = D3DXVECTOR4(0.2, 0.2, 0.2, 1.0);
	D3DXVECTOR4 Kd = D3DXVECTOR4(0.7, 0.5, 0.6, 1.0);
	D3DXVECTOR4 Ks = D3DXVECTOR4(1.0, 1.0, 1.0, 1.0);
	m_Camera->getPosition(&camerapos);
	realcamerpos = D3DXVECTOR4(camerapos.x, camerapos.y, camerapos.z, 1.0);
	//把cube顶点和索引数据放入缓冲区，准备渲染
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	//用light shader渲染
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetPosition(), m_Light->GetLightColor(), m_Light->GetGlobalAmbient(),
		realcamerpos, Ke, Ka, Kd, Ks, m_Light->GetDirection(), m_Light->GetShininess());
	if (!result)
	{
		return false;
	}

	Ke = D3DXVECTOR4(0.2, 0.8, 0.0, 1.0);
	Ka = D3DXVECTOR4(0.3, 0.3, 0.3, 1.0);
	Kd = D3DXVECTOR4(1.0, 1.0, 1.0, 1.0);
	Ks = D3DXVECTOR4(1.0, 1.0, 1.0, 1.0);

	//把cube顶点和索引数据放入缓冲区，准备渲染
	m_PlaneModel->Render(m_D3D->GetDeviceContext());
	//用light shader渲染
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetPosition(), m_Light->GetLightColor(), m_Light->GetGlobalAmbient(),
		realcamerpos, Ke, Ka, Kd, Ks, m_Light->GetDirection(), m_Light->GetShininess());
	if (!result)
	{
		return false;
	}

	//把framebuffer中的图像present到屏幕上.
	m_D3D->EndScene();


	return true;
}

void GraphicsClass::pick(int x, int y)
{
	sRay ray = m_D3D->calculate_picking_ray(x, y);
	D3DXMATRIX view_matrix, view_inverse_matrix;
	m_Camera->getViewMatrix(&view_matrix);

	D3DXMatrixInverse(&view_inverse_matrix, NULL, &view_matrix);

	transform_ray(&ray, &view_inverse_matrix);

	bool b = m_CubeModel->InsectRay(&ray);

	if (b)
	{
		DPrintf("Insection ...");
	}
}
