#include "PlaneModelClass.h"


PlaneModelClass::PlaneModelClass(void)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

PlaneModelClass::PlaneModelClass(const PlaneModelClass& others)
{

}

PlaneModelClass::~PlaneModelClass(void)
{
}

bool PlaneModelClass::Initialize(ID3D11Device* device)
{
	bool result;


	// 初始化顶点缓冲和顶点索引缓冲.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void PlaneModelClass::Shutdown()
{
	// 释放顶点和索引缓冲.
	ShutdownBuffers();

	return;
}

void PlaneModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// 把顶点和索引缓冲放入图形管线，准备渲染.
	RenderBuffers(deviceContext);

	return;
}

int PlaneModelClass::GetIndexCount()
{
	//返回索引顶点计数
	return m_indexCount;
}

bool PlaneModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//首先，我们创建2个临时缓冲存放顶点和索引数据，以便后面使用。. 

	// 设置顶点缓冲大小为4，一个平面.
	m_vertexCount = 4;

	// 设置索引缓冲大小.,两个三角形
	m_indexCount = 6;

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
	vertices[0].position = D3DXVECTOR3(-50.0f, -3.0f, -50.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(-50.0f, -3.0f, 50.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(50.0f, -3.0f, 50.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	vertices[3].position = D3DXVECTOR3(50.0f, -3.0f, -50.0f);
	vertices[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


	// 设置索引缓冲数据.
	indices[0] = 0;  // 前面
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

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
		HR(result);
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
		HR(result);
		return false;
	}

	// 释放临时缓冲.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void PlaneModelClass::ShutdownBuffers()
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

void PlaneModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

