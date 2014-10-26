#include "CubeModelClass.h"


CubeModelClass::CubeModelClass()
{
	m_vertexCount = 0;
	m_indexCount = 0;
}

CubeModelClass::CubeModelClass(const CubeModelClass & src)
{

}

CubeModelClass::~CubeModelClass()
{
}

bool CubeModelClass::Initialize(ID3D11Device* device)
{
	bool result = InitializeBuffers(device);
	return result;
}

void CubeModelClass::Shutdown()
{
	ShutdownBuffers();
}

void CubeModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int CubeModelClass::GetIndexCount()
{
	return m_indexCount;
}

bool CubeModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType *vertices;
	unsigned long *indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//首先，我们创建2个临时缓冲存放顶点和索引数据，以便后面使用。. 

	// 设置顶点缓冲大小为8，一个正方体.
	m_vertexCount = 8;

	// 设置索引缓冲大小.
	m_indexCount = 36;

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
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	vertices[0].normal = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);

	vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	vertices[1].normal = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	vertices[2].normal = D3DXVECTOR3(1.0f, 1.0f, -1.0f);

	vertices[3].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	vertices[3].normal = D3DXVECTOR3(1.0f, -1.0f, -1.0f);

	vertices[4].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	vertices[4].normal = D3DXVECTOR3(1.0f, -1.0f, -1.0f);

	vertices[5].position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vertices[5].normal = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);

	vertices[6].position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertices[6].normal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	vertices[7].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	vertices[7].normal = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	// 设置索引缓冲数据.
	indices[0] = 0;  // 前面
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	indices[6] = 4;  // 后面
	indices[7] = 6;
	indices[8] = 5;
	indices[9] = 4;
	indices[10] = 7;
	indices[11] = 6;

	indices[12] = 4;  // 左面
	indices[13] = 5;
	indices[14] = 1;
	indices[15] = 4;
	indices[16] = 1;
	indices[17] = 0;

	indices[18] = 3;  //右面
	indices[19] = 2;
	indices[20] = 6;
	indices[21] = 3;
	indices[22] = 6;
	indices[23] = 7;

	indices[24] = 1;  // 上面
	indices[25] = 5;
	indices[26] = 6;
	indices[27] = 1;
	indices[28] = 6;
	indices[29] = 2;

	indices[30] = 4;  // 下面
	indices[31] = 0;
	indices[32] = 3;
	indices[33] = 4;
	indices[34] = 3;
	indices[35] = 7;


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

	D3DXComputeBoundingSphere((D3DXVECTOR3*)vertices, sizeof(vertices), sizeof(VertexType), &m_boundingSphere.m_center, &m_boundingSphere.m_radius);


	// 释放临时缓冲.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void CubeModelClass::ShutdownBuffers()
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

void CubeModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

bool CubeModelClass::InsectRay(sRay* ray)
{
	D3DXVECTOR3 v = ray->origin - m_boundingSphere.m_center;

	float b = 2.0f * D3DXVec3Dot(&ray->direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (m_boundingSphere.m_radius * m_boundingSphere.m_radius);

	float discriminant = (b * b) - (4.0f * c);

	if (discriminant < 0.0f)
		return false;

	discriminant = sqrt(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;

	// if one solution is >= 0, then we intersected the sphere.
	bool bInscetion = (s0 >= 0.0f || s1 >= 0.0f);
	return bInscetion;
}
