#include "ModelClass.h"


ModelClass::ModelClass(void)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
}

ModelClass::ModelClass(const ModelClass & others)
{

}

ModelClass::~ModelClass(void)
{
}

bool ModelClass::Initialize(ID3D11Device* device, WCHAR* textureFilename)
{
	bool result;

	// 初始化顶点缓冲和顶点索引缓冲.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	// 装入纹理.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}


	return true;
}

void ModelClass::Shutdown()
{

	//释放纹理.
	ReleaseTexture();

	// 释放顶点和索引缓冲.
	ShutdownBuffers();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// 把顶点和索引缓冲放入图形管线，准备渲染.
	RenderBuffers(deviceContext);

	return;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

int ModelClass::GetIndexCount()
{
	//返回索引顶点计数
	return m_indexCount;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//首先，我们创建2个临时缓冲存放顶点和索引数据，以便后面使用。. 

	// 设置顶点缓冲大小为3，一个三角形.
	m_vertexCount = 30;

	// 设置索引缓冲大小.
	m_indexCount = 30;

	// 创建顶点临时缓冲.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// 创建索引缓冲.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}
	//创建顺时针方向的三角形，左手规则
	// 设置顶点数据.
	// Load the vertex array with data.
	vertices[0].position = D3DXVECTOR3(-3.0f, -1.0f, 0.0f);
	vertices[0].texture = D3DXVECTOR2(-1.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(-3.0f, 1.0f, 0.0f);
	vertices[1].texture = D3DXVECTOR2(-1.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertices[2].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[3].position = D3DXVECTOR3(-3.0f, -1.0f, 0.0f);
	vertices[3].texture = D3DXVECTOR2(-1.0f, 1.0f);

	vertices[4].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertices[4].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertices[5].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[6].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertices[6].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[7].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertices[7].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[8].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertices[8].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[9].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertices[9].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[10].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertices[10].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[11].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertices[11].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[12].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertices[12].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[13].position = D3DXVECTOR3(-1.0f, 3.0f, 0.0f);
	vertices[13].texture = D3DXVECTOR2(0.0f, -1.0f);

	vertices[14].position = D3DXVECTOR3(1.0f, 3.0f, 0.0f);
	vertices[14].texture = D3DXVECTOR2(1.0f, -1.0f);

	vertices[15].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertices[15].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[16].position = D3DXVECTOR3(1.0f, 3.0f, 0.0f);
	vertices[16].texture = D3DXVECTOR2(1.0f, -1.0f);

	vertices[17].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertices[17].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[18].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertices[18].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[19].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertices[19].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[20].position = D3DXVECTOR3(3.0f, 1.0f, 0.0f);
	vertices[20].texture = D3DXVECTOR2(2.0f, 0.0f);

	vertices[21].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertices[21].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[22].position = D3DXVECTOR3(3.0f, 1.0f, 0.0f);
	vertices[22].texture = D3DXVECTOR2(2.0f, 0.0f);

	vertices[23].position = D3DXVECTOR3(3.0f, -1.0f, 0.0f);
	vertices[23].texture = D3DXVECTOR2(2.0f, 1.0f);

	vertices[24].position = D3DXVECTOR3(-1.0f, -3.0f, 0.0f);
	vertices[24].texture = D3DXVECTOR2(0.0f, 2.0f);

	vertices[25].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertices[25].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[26].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertices[26].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[27].position = D3DXVECTOR3(-1.0f, -3.0f, 0.0f);
	vertices[27].texture = D3DXVECTOR2(0.0f, 2.0f);

	vertices[28].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertices[28].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[29].position = D3DXVECTOR3(1.0f, -3.0f, 0.0f);
	vertices[29].texture = D3DXVECTOR2(1.0f, 2.0f);
	// 设置索引缓冲数据.
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// 设置顶点缓冲描述
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 指向保存顶点数据的临时缓冲.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 创建顶点缓冲.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 设置索引缓冲描述.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 指向存临时索引缓冲.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 创建索引缓冲.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 释放临时缓冲.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// 释放顶点缓冲.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// 释放索引缓冲
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// 设置顶点缓冲跨度和偏移.
	stride = sizeof(VertexType);
	offset = 0;

	//在input assemberl阶段绑定顶点缓冲，以便能够被渲染
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//在input assemberl阶段绑定索引缓冲，以便能够被渲染
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 设置体元语义，渲染三角形列表.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// 创建纹理
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// 创建纹理对象.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	// 释放纹理对象
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

