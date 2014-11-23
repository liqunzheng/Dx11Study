#include "ColorShaderClass.h"


ColorShaderClass::ColorShaderClass(void)
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;

	m_sampleState = 0;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other)
{
}


ColorShaderClass::~ColorShaderClass(void)
{
}

bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// ��ʼ��vs ��ps.
	result = InitializeShader(device, hwnd, L"color.vs", L"color.ps");
	if (!result)
	{
		return false;
	}

	return true;
}
void ColorShaderClass::Shutdown()
{
	// �ͷ�shader��Դ.
	ShutdownShader();

	return;
}
bool ColorShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	bool result;


	// ����shader����.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result)
	{
		return false;
	}

	// ��shader��Ⱦָ�����嶥��
	RenderShader(deviceContext, indexCount);

	return true;
}

bool ColorShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;


	// ��ʼ��ָ��Ϊ��.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// ����vs����.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// ���vs����ʧ�ܣ����������Ϣ.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// ���û���κδ�����Ϣ��������shader�ļ���ʧ.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// ����ps.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// ���ps����ʧ�ܣ����������Ϣ.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// ���û���κδ�����Ϣ��������shader�ļ���ʧ.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}


	// �ӻ��崴��vs shader.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// �ӻ��崴��ps shader.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// �������ݲ��֣��Ա���shader��ʹ��.
	// ����Ҫ��ModelClass�еĶ���ṹһ��.
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
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// �����������벼��.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
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
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// ����const bufferָ�룬�Ա����shader����.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// �����������������
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
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
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}


	return true;
}

void ColorShaderClass::ShutdownShader()
{

	// �ͷŲ���״̬
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

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

	return;
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// �õ�������Ϣbufferָ��.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// �õ�buffer�ĳ���.
	bufferSize = errorMessage->GetBufferSize();

	// ��һ���ļ�д������Ϣ.
	fout.open("shader-error.txt");

	// �����������.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// �ر��ļ�
	fout.close();

	// �ͷŴ�����Ϣ.
	errorMessage->Release();
	errorMessage = 0;

	//����������ʾ.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// ����shaderǰ��ȷ������ת�ã�����D3D11��Ҫ��.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	//  �����������壬�Ա��ܹ�д��.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// �õ�const bufferָ��.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// ����world,view�Լ�projection����.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// ������������.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// ���ó�������λ��.
	bufferNumber = 0;

	// �ø��º��ֵ���ó�������.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	// ����ps shader��Դ.
	deviceContext->PSSetShaderResources(0, 1, &texture);


	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// �󶨶��㲼��.
	deviceContext->IASetInputLayout(m_layout);

	// ������Ⱦʹ��vs��ps.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	//���ò���״̬
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// ��Ⱦ������
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
