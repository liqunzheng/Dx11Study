#include "stdafx.h"
#include "MtlShader.h"
#include "D3D11Class.h"


CMtlShader::CMtlShader()
{
	m_strName = L"CMtlShader";
	m_strPSName = L"Material.ps";
	m_strVSName = L"Material.vs";

	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
}


CMtlShader::~CMtlShader()
{
	Shutdown();
}

bool CMtlShader::Initialize(CD3D11Class* p3DRoot)
{
	ID3D10Blob* errorMessage = 0;
	std::wstring strErrorMessage;

	// 编译vs代码.
	ID3D10Blob* vertexShaderBuffer = 0;
	HRESULT result = D3DX11CompileFromFile(m_strVSName.c_str(), NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		// 如果vs编译失败，输出错误消息.
		if (errorMessage)
		{
			char* compileErrors = (char*)(errorMessage->GetBufferPointer());
			ULONG nSize = errorMessage->GetBufferSize();
			DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, compileErrors, -1, NULL, 0);
			wchar_t *pwText = new wchar_t[dwNum];
			MultiByteToWideChar(CP_ACP, 0, compileErrors, -1, pwText, dwNum);
			strErrorMessage = pwText;
		}
		// 如果没有任何错误消息，可能是shader文件丢失.
		else
		{
			strErrorMessage = L"Missing Shader File";
		}
		return false;
	}

	// 编译ps.
	ID3D10Blob* pixelShaderBuffer = 0;
	result = D3DX11CompileFromFile(m_strPSName.c_str(), NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// 如果vs编译失败，输出错误消息.
		if (errorMessage)
		{
			char* compileErrors = (char*)(errorMessage->GetBufferPointer());
			ULONG nSize = errorMessage->GetBufferSize();
			DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, compileErrors, -1, NULL, 0);
			wchar_t *pwText = new wchar_t[dwNum];
			MultiByteToWideChar(CP_ACP, 0, compileErrors, -1, pwText, dwNum);
			strErrorMessage = pwText;
		}
		// 如果没有任何错误消息，可能是shader文件丢失.
		else
		{
			strErrorMessage = L"Missing Shader File";
		}
		return false;
	}

	// 从缓冲创建vs shader.
	result = p3DRoot->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	// 从缓冲创建ps shader.
	result = p3DRoot->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// 设置数据布局，以便在shader中使用.
	// 定义要和ModelClass中的顶点结构一致.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";//vs中的输入参数 
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

	// 得到layout中的元素数量
	int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// 创建顶点输入布局.
	result = p3DRoot->GetDevice()->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	//释放顶点和像素缓冲.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// 设置动态矩阵描述.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// 创建const buffer指针，以便访问shader常量.
	result = p3DRoot->GetDevice()->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 创建纹理采样描述符
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

	// 创建纹理采样状态.
	result = p3DRoot->GetDevice()->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void CMtlShader::Shutdown()
{
	// 释放采样状态
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// 释放常量缓冲
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	//释放顶点布局t.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// 释放ps .
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// 释放vs
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

bool CMtlShader::Render(CD3D11Class* p3DRoot, UINT indexCount)
{
	// 绑定顶点布局.
	p3DRoot->GetDeviceContext()->IASetInputLayout(m_layout);

	// 设置渲染使用vs和ps.
	p3DRoot->GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	p3DRoot->GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	//设置采样状态
	p3DRoot->GetDeviceContext()->PSSetSamplers(0, 1, &m_sampleState);

	// 渲染三角形
	p3DRoot->GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
	
	return true;
}

bool CMtlShader::SetMatrix(CD3D11Class* p3DRoot, D3DXMATRIX mxWorld, D3DXMATRIX mxView, D3DXMATRIX mxProject)
{
	// 传入shader前，确保矩阵转置，这是D3D11的要求.
	D3DXMatrixTranspose(&mxWorld, &mxWorld);
	D3DXMatrixTranspose(&mxView, &mxView);
	D3DXMatrixTranspose(&mxProject, &mxProject);

	//  锁定常量缓冲，以便能够写入.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = p3DRoot->GetDeviceContext()->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// 得到const buffer指针.
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	// 设置world,view以及projection矩阵.
	dataPtr->world = mxWorld;
	dataPtr->view = mxView;
	dataPtr->projection = mxProject;

	// 解锁常量缓冲.
	p3DRoot->GetDeviceContext()->Unmap(m_matrixBuffer, 0);

	// 用更新后的值设置常量缓冲.
	p3DRoot->GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_matrixBuffer);
	
	return true;
}

bool CMtlShader::SetShaderResource(CD3D11Class* p3DRoot, ID3D11ShaderResourceView* pResource)
{
	// 设置ps shader资源.
	p3DRoot->GetDeviceContext()->PSSetShaderResources(0, 1, &pResource);
	return true;
}

