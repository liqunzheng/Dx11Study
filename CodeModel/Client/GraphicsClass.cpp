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

	//��������Ѿ����ڣ����ͷŵ�����
	Shutdown();

	// ����һ��D3DClass����.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// ����D3DClass��ʼ������
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

	//�������������
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// ���������λ��.
	D3DXVECTOR3 campos = D3DXVECTOR3(8.0f, 3.0f, -20.0f);
	m_Camera->setPosition(&campos);

	// ����ģ�Ͷ���.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// ��ʼ��ģ�Ͷ���.
	result = m_Model->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// ���Ὠģ�Ͷ���.
	m_AxisModel = new AxisModelClass;
	if (!m_AxisModel)
	{
		return false;
	}
	// ��ʼ��������ģ�Ͷ���.
	result = m_AxisModel->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the axis model object.", L"Error", MB_OK);
		return false;
	}

	// ��ƽ��ģ�Ͷ���.
	m_PlaneModel = new PlaneModelClass;
	if (!m_PlaneModel)
	{
		return false;
	}
	// ��ʼ��ƽ��ģ�Ͷ���.
	result = m_PlaneModel->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the plane model object.", L"Error", MB_OK);
		return false;
	}

	// ����cubeģ�Ͷ���
	m_CubeModel = new CubeModelClass;
	if (!m_CubeModel)
	{
		return false;
	}
	// ��ʼ������cubeģ�Ͷ���.
	result = m_CubeModel->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the axis model object.", L"Error", MB_OK);
		return false;
	}

	// ����shader����
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// ��ʼ��shader����
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// ��������shader��
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	//��ʼ������shader����
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// ������Դ����.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{

	// �ͷŹ�Դ����.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// �ͷŹ��ղ���shader����.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// �ͷ�shader����.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// �ͷ�ģ�Ͷ���.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
	// �ͷ�������ģ�Ͷ���.
	if (m_AxisModel)
	{
		m_AxisModel->Shutdown();
		delete m_AxisModel;
		m_AxisModel = 0;
	}

	//�ͷ�planeģ�Ͷ���
	if (m_PlaneModel)
	{
		m_PlaneModel->Shutdown();
		delete m_PlaneModel;
		m_PlaneModel = 0;
	}

	//�ͷ�cubeģ�Ͷ���
	if (m_CubeModel)
	{
		m_CubeModel->Shutdown();
		delete m_CubeModel;
		m_CubeModel = 0;
	}

	// �ͷ����������
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	//����m_D3D����
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


	// ����Render��������Ⱦ3D����
	// Render��GraphicsClass��˽�к���.
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


	// ����framebuffer.Ϊǳ��ɫ
	m_D3D->BeginScene(0.0f, 0.0f, 0.5f, 0.5f);

	// �õ�3������.
	m_Camera->getViewMatrix(&viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_AxisModel->Render(m_D3D->GetDeviceContext());
	// ��shader��Ⱦ.
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_AxisModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	// ��ģ�Ͷ�����������������ߣ�׼����Ⱦ.
	m_Model->Render(m_D3D->GetDeviceContext());

	// ��shader��Ⱦ.
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}


	//�õ������λ�ú�һЩ���ʹ���ϵ��
	D3DXVECTOR3 camerapos;
	D3DXVECTOR4 realcamerpos;
	D3DXVECTOR4 Ke = D3DXVECTOR4(0.8, 0.0, 0.2, 1.0);
	D3DXVECTOR4 Ka = D3DXVECTOR4(0.2, 0.2, 0.2, 1.0);
	D3DXVECTOR4 Kd = D3DXVECTOR4(0.7, 0.5, 0.6, 1.0);
	D3DXVECTOR4 Ks = D3DXVECTOR4(1.0, 1.0, 1.0, 1.0);
	m_Camera->getPosition(&camerapos);
	realcamerpos = D3DXVECTOR4(camerapos.x, camerapos.y, camerapos.z, 1.0);
	//��cube������������ݷ��뻺������׼����Ⱦ
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	//��light shader��Ⱦ
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

	//��cube������������ݷ��뻺������׼����Ⱦ
	m_PlaneModel->Render(m_D3D->GetDeviceContext());
	//��light shader��Ⱦ
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetPosition(), m_Light->GetLightColor(), m_Light->GetGlobalAmbient(),
		realcamerpos, Ke, Ka, Kd, Ks, m_Light->GetDirection(), m_Light->GetShininess());
	if (!result)
	{
		return false;
	}

	//��framebuffer�е�ͼ��present����Ļ��.
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
