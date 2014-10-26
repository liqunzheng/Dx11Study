#include "LightShaderClass.h"


LightShaderClass::LightShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_lightmaterialBuffer = 0;
}


LightShaderClass::~LightShaderClass()
{
}

bool LightShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//初始化vs和ps
	result = InitializeShader(device, hwnd, L"light.vs", L"light.ps");
	if (!result)
	{
		return false;
	}
	return true;
}

void LightShaderClass::Shutdown()
{
	ShutdownShader();
}

bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix, D3DXVECTOR3 cameraPosition)
{
	bool result;
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, cameraPosition);
	if (!result)
	{
		HR(result);
		return false;
	}

	RenderShader(deviceContext, indexCount);
	return true;
}

bool LightShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob *errorMessage;
	ID3D10Blob *vertexShaderBuffer;
	ID3D10Blob *pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightmaterialBufferDesc;

	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// 如果没有任何错误消息，可能是shader文件丢失.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//  编译ps代码.

	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// 如果ps编译失败，输出错误信息.
		HR(result);
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// 如果没有任何错误消息，可能是shader文件丢失.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// 从缓冲创建vs shader.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// 从缓冲中创建ps shader.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// 设置数据布局，以便在shader中使用.
	// 定义要和ModelClass中的顶点结构一致.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// 得到layout中的元素数量
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// 创建顶点输入布局.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	//释放顶点和像素缓冲.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	// 设置动态矩阵描述.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	lightmaterialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightmaterialBufferDesc.ByteWidth = sizeof(LightMaterialBufferType);
	lightmaterialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightmaterialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightmaterialBufferDesc.MiscFlags = 0;
	lightmaterialBufferDesc.StructureByteStride = 0;

	// 创建const buffer指针，以便访问shader常量.
	result = device->CreateBuffer(&lightmaterialBufferDesc, NULL, &m_lightmaterialBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	return true;
}

void LightShaderClass::ShutdownShader()
{
	// 释放光照材质缓冲.
	if (m_lightmaterialBuffer)
	{
		m_lightmaterialBuffer->Release();
		m_lightmaterialBuffer = 0;
	}

	// 释放常量缓冲
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	//释放顶点布局.
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

void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// 得到错误信息buffer指针.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// 得到buffer的长度.
	bufferSize = errorMessage->GetBufferSize();

	// 打开一个文件写错误信息.
	fout.open("shader-error.txt");

	// 输出错误信心.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// 关闭文件
	fout.close();

	// 释放错误消息.
	errorMessage->Release();
	errorMessage = 0;

	//弹出错误提示.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
}

bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix, D3DXVECTOR3 cameraPosition)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType *dataPtr;
	LightMaterialBufferType *dataPtr2;

	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	dataPtr = (MatrixBufferType *)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap(m_matrixBuffer, 0);
	bufferNumber = 0;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	result = deviceContext->Map(m_lightmaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (LightMaterialBufferType *)mappedResource.pData;
	dataPtr2->cameraPosition = cameraPosition;
	deviceContext->Unmap(m_lightmaterialBuffer, 0);

	bufferNumber = 1;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_lightmaterialBuffer);
	
	return true;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// 绑定顶点布局.
	deviceContext->IASetInputLayout(m_layout);

	// 设置渲染使用vs和ps.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// 渲染三角形
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

