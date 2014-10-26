#include "GraphicsClass.h"


GraphicsClass::GraphicsClass(void)
{
	m_D3D = 0;
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

	//�������������
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// ���������λ��.
	D3DXVECTOR3 campos = D3DXVECTOR3(0.0f, 0.0f, -10.0f);
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

	//����������ģ�Ͷ���
	m_AxisModel = new AxisModelClass;
	if (!m_AxisModel)
	{
		return false;
	}
	// ��ʼ��������ģ�Ͷ���
	result = m_AxisModel->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the axis model object.", L"Error", MB_OK);
		return false;
	}

	// ��ʼ��ƽ��ģ�Ͷ���.
	m_PlaneModel = new PlaneModelClass;
	if (!m_PlaneModel)
	{
		return false;
	}
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

	//���ù�Դλ�ã������Ĳ���ʹ��Ĭ��ֵ
	m_Light->SetLightPosition(5.0f, 3.0f, -1.0f);

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
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	// ����framebuffer.Ϊǳ��ɫ
	m_D3D->BeginScene(0.0f, 0.0f, 0.5f, 1.0f);

	// �õ�3������.
	m_Camera->getViewMatrix(&viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	//��Ⱦ������
	m_AxisModel->Render(m_D3D->GetDeviceContext());
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
	D3DXVECTOR3 Ke = D3DXVECTOR3(1.0, 0.0, 0.0);
	D3DXVECTOR3 Ka = D3DXVECTOR3(1.0, 1.0, 1.0);
	D3DXVECTOR3 Kd = D3DXVECTOR3(1.0, 1.0, 1.0);
	D3DXVECTOR3 Ks = D3DXVECTOR3(1.0, 1.0, 1.0);
	m_Camera->getPosition(&camerapos);

	//��cube������������ݷ��뻺������׼����Ⱦ
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	//��light shader��Ⱦ
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		camerapos);
	if (!result)
	{
		return false;
	}

	//��cube������������ݷ��뻺������׼����Ⱦ
	m_PlaneModel->Render(m_D3D->GetDeviceContext());
	//��light shader��Ⱦ
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		camerapos);
	if (!result)
	{
		return false;
	}

	//��framebuffer�е�ͼ��present����Ļ��.
	m_D3D->EndScene();

	return true;
}