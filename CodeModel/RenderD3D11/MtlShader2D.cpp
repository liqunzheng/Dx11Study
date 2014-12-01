#include "stdafx.h"
#include "MtlShader2D.h"
#include "D3D11Class.h"


CMtlShader2D::CMtlShader2D()
{
	m_strName = L"CMtlShader2D";
	m_strPSName = L"Material2D.ps";
	m_strVSName = L"Material2D.vs";

	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
}


CMtlShader2D::~CMtlShader2D()
{
	Shutdown();
}

bool CMtlShader2D::Initialize(CD3D11Class* p3DRoot)
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

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
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

	// �����������������
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 1.0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 1.0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// �����������״̬.
	result = p3DRoot->GetDevice()->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void CMtlShader2D::Shutdown()
{
	// �ͷŲ���״̬
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	//�ͷŶ��㲼��
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

bool CMtlShader2D::Render(CD3D11Class* p3DRoot, UINT indexCount)
{
	// �󶨶��㲼��.
	p3DRoot->GetDeviceContext()->IASetInputLayout(m_layout);

	// ������Ⱦʹ��vs��ps.
	p3DRoot->GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	p3DRoot->GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	//���ò���״̬
	p3DRoot->GetDeviceContext()->PSSetSamplers(0, 1, &m_sampleState);

	// ��Ⱦ������
	p3DRoot->GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
	
	return true;
}

bool CMtlShader2D::SetMatrix(CD3D11Class* p3DRoot, D3DXMATRIX mxWorld, D3DXMATRIX mxView, D3DXMATRIX mxProject)
{
	return true;
}

bool CMtlShader2D::SetShaderResource(CD3D11Class* p3DRoot, ID3D11ShaderResourceView* pResource)
{
	// ����ps shader��Դ.
	p3DRoot->GetDeviceContext()->PSSetShaderResources(0, 1, &pResource);
	return true;
}

