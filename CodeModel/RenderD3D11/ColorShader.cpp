#include "stdafx.h"
#include "ColorShader.h"
#include "D3D11Class.h"


CColorShader::CColorShader()
{
	m_strName = L"CColorShader";
	m_strPSName = L"color.ps";
	m_strVSName = L"color.vs";

	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}


CColorShader::~CColorShader()
{
	Shutdown();
}

bool CColorShader::Initialize(CD3D11Class* p3DRoot)
{
	ID3D10Blob* errorMessage = 0;
	std::wstring strErrorMessage;

	// ����vs����.
	ID3D10Blob* vertexShaderBuffer = 0;
	HRESULT result = D3DX11CompileFromFile(m_strVSName.c_str(), NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		// ���vs����ʧ�ܣ����������Ϣ.
		if (errorMessage)
		{
			char* compileErrors = (char*)(errorMessage->GetBufferPointer());
			ULONG nSize = errorMessage->GetBufferSize();
			DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, compileErrors, -1, NULL, 0);
			wchar_t *pwText = new wchar_t[dwNum];
			MultiByteToWideChar(CP_ACP, 0, compileErrors, -1, pwText, dwNum);
			strErrorMessage = pwText;
		}
		// ���û���κδ�����Ϣ��������shader�ļ���ʧ.
		else
		{
			strErrorMessage = L"Missing Shader File";
		}
		return false;
	}

	// ����ps.
	ID3D10Blob* pixelShaderBuffer = 0;
	result = D3DX11CompileFromFile(m_strPSName.c_str(), NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);

	HR(result);
	if (FAILED(result))
	{
		// ���vs����ʧ�ܣ����������Ϣ.
		if (errorMessage)
		{
			char* compileErrors = (char*)(errorMessage->GetBufferPointer());
			ULONG nSize = errorMessage->GetBufferSize();
			DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, compileErrors, -1, NULL, 0);
			wchar_t *pwText = new wchar_t[dwNum];
			MultiByteToWideChar(CP_ACP, 0, compileErrors, -1, pwText, dwNum);
			strErrorMessage = pwText;
		}
		// ���û���κδ�����Ϣ��������shader�ļ���ʧ.
		else
		{
			strErrorMessage = L"Missing Shader File";
		}
		return false;
	}

	// �ӻ��崴��vs shader.
	result = p3DRoot->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	// �ӻ��崴��ps shader.
	result = p3DRoot->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// �������ݲ��֣��Ա���shader��ʹ��.
	// ����Ҫ��ModelClass�еĶ���ṹһ��.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";//vs�е�������� 
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// �õ�layout�е�Ԫ������
	int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// �����������벼��.
	result = p3DRoot->GetDevice()->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	//�ͷŶ�������ػ���.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// ���ö�̬��������.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// ����const bufferָ�룬�Ա����shader����.
	result = p3DRoot->GetDevice()->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void CColorShader::Shutdown()
{
	// �ͷų�������
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	//�ͷŶ��㲼��t.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// �ͷ�ps .
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// �ͷ�vs
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

bool CColorShader::Render(CD3D11Class* p3DRoot, UINT indexCount)
{
	// �󶨶��㲼��.
	p3DRoot->GetDeviceContext()->IASetInputLayout(m_layout);

	// ������Ⱦʹ��vs��ps.
	p3DRoot->GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	p3DRoot->GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	// ��Ⱦ������
	p3DRoot->GetDeviceContext()->DrawIndexed(indexCount, 0, 0);

	return true;
}

bool CColorShader::SetMatrix(CD3D11Class* p3DRoot, D3DXMATRIX mxWorld, D3DXMATRIX mxView, D3DXMATRIX mxProject)
{
	// ����shaderǰ��ȷ������ת�ã�����D3D11��Ҫ��.
	D3DXMatrixTranspose(&mxWorld, &mxWorld);
	D3DXMatrixTranspose(&mxView, &mxView);
	D3DXMatrixTranspose(&mxProject, &mxProject);

	//  �����������壬�Ա��ܹ�д��.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = p3DRoot->GetDeviceContext()->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// �õ�const bufferָ��.
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	// ����world,view�Լ�projection����.
	dataPtr->world = mxWorld;
	dataPtr->view = mxView;
	dataPtr->projection = mxProject;

	// ������������.
	p3DRoot->GetDeviceContext()->Unmap(m_matrixBuffer, 0);

	// �ø��º��ֵ���ó�������.
	p3DRoot->GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

	return true;
}

bool CColorShader::SetShaderResource(CD3D11Class*, ID3D11ShaderResourceView*)
{
	return true;
}
