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

	HR(result);
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

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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

	return true;
}

void CColorShader::Shutdown()
{
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

bool CColorShader::Render(CD3D11Class* p3DRoot, UINT indexCount)
{
	// 绑定顶点布局.
	p3DRoot->GetDeviceContext()->IASetInputLayout(m_layout);

	// 设置渲染使用vs和ps.
	p3DRoot->GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	p3DRoot->GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	// 渲染三角形
	p3DRoot->GetDeviceContext()->DrawIndexed(indexCount, 0, 0);

	return true;
}

bool CColorShader::SetMatrix(CD3D11Class* p3DRoot, D3DXMATRIX mxWorld, D3DXMATRIX mxView, D3DXMATRIX mxProject)
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

bool CColorShader::SetShaderResource(CD3D11Class*, ID3D11ShaderResourceView*)
{
	return true;
}
