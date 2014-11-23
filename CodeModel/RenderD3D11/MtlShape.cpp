#include "stdafx.h"
#include "MtlShape.h"
#include "D3D11Class.h"


CMtlShape::CMtlShape()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


CMtlShape::~CMtlShape()
{
	Shutdown();
}

bool CMtlShape::Initialize(CD3D11Class* p3DRoot)
{
	m_strShaderName = L"CMtlShader";
	m_strMtlName = L"tong.dds";
	D3DXMatrixIdentity(&m_mxWorld);

	m_indices.resize(30);
	m_vertices.resize(30);

	//创建顺时针方向的三角形，左手规则
	// 设置顶点数据.
	m_vertices[0].position = D3DXVECTOR3(-3.0f, -1.0f, 0.0f);
	m_vertices[0].texture = D3DXVECTOR2(-1.0f, 1.0f);

	m_vertices[1].position = D3DXVECTOR3(-3.0f, 1.0f, 0.0f);
	m_vertices[1].texture = D3DXVECTOR2(-1.0f, 0.0f);

	m_vertices[2].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_vertices[2].texture = D3DXVECTOR2(0.0f, 0.0f);

	m_vertices[3].position = D3DXVECTOR3(-3.0f, -1.0f, 0.0f);
	m_vertices[3].texture = D3DXVECTOR2(-1.0f, 1.0f);

	m_vertices[4].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_vertices[4].texture = D3DXVECTOR2(0.0f, 0.0f);

	m_vertices[5].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	m_vertices[5].texture = D3DXVECTOR2(0.0f, 1.0f);

	m_vertices[6].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	m_vertices[6].texture = D3DXVECTOR2(0.0f, 1.0f);

	m_vertices[7].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_vertices[7].texture = D3DXVECTOR2(0.0f, 0.0f);

	m_vertices[8].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_vertices[8].texture = D3DXVECTOR2(1.0f, 0.0f);

	m_vertices[9].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	m_vertices[9].texture = D3DXVECTOR2(0.0f, 1.0f);

	m_vertices[10].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_vertices[10].texture = D3DXVECTOR2(1.0f, 0.0f);

	m_vertices[11].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	m_vertices[11].texture = D3DXVECTOR2(1.0f, 1.0f);

	m_vertices[12].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_vertices[12].texture = D3DXVECTOR2(0.0f, 0.0f);

	m_vertices[13].position = D3DXVECTOR3(-1.0f, 3.0f, 0.0f);
	m_vertices[13].texture = D3DXVECTOR2(0.0f, -1.0f);

	m_vertices[14].position = D3DXVECTOR3(1.0f, 3.0f, 0.0f);
	m_vertices[14].texture = D3DXVECTOR2(1.0f, -1.0f);

	m_vertices[15].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_vertices[15].texture = D3DXVECTOR2(0.0f, 0.0f);

	m_vertices[16].position = D3DXVECTOR3(1.0f, 3.0f, 0.0f);
	m_vertices[16].texture = D3DXVECTOR2(1.0f, -1.0f);

	m_vertices[17].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_vertices[17].texture = D3DXVECTOR2(1.0f, 0.0f);

	m_vertices[18].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	m_vertices[18].texture = D3DXVECTOR2(1.0f, 1.0f);

	m_vertices[19].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_vertices[19].texture = D3DXVECTOR2(1.0f, 0.0f);

	m_vertices[20].position = D3DXVECTOR3(3.0f, 1.0f, 0.0f);
	m_vertices[20].texture = D3DXVECTOR2(2.0f, 0.0f);

	m_vertices[21].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	m_vertices[21].texture = D3DXVECTOR2(1.0f, 1.0f);

	m_vertices[22].position = D3DXVECTOR3(3.0f, 1.0f, 0.0f);
	m_vertices[22].texture = D3DXVECTOR2(2.0f, 0.0f);

	m_vertices[23].position = D3DXVECTOR3(3.0f, -1.0f, 0.0f);
	m_vertices[23].texture = D3DXVECTOR2(2.0f, 1.0f);

	m_vertices[24].position = D3DXVECTOR3(-1.0f, -3.0f, 0.0f);
	m_vertices[24].texture = D3DXVECTOR2(0.0f, 2.0f);

	m_vertices[25].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	m_vertices[25].texture = D3DXVECTOR2(0.0f, 1.0f);

	m_vertices[26].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	m_vertices[26].texture = D3DXVECTOR2(1.0f, 1.0f);

	m_vertices[27].position = D3DXVECTOR3(-1.0f, -3.0f, 0.0f);
	m_vertices[27].texture = D3DXVECTOR2(0.0f, 2.0f);

	m_vertices[28].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	m_vertices[28].texture = D3DXVECTOR2(1.0f, 1.0f);

	m_vertices[29].position = D3DXVECTOR3(1.0f, -3.0f, 0.0f);
	m_vertices[29].texture = D3DXVECTOR2(1.0f, 2.0f);

	// 设置索引缓冲数据.
	// 设置索引缓冲数据.
	for (size_t i = 0; i < m_indices.size(); i++)
	{
		m_indices[i] = i;
	}


	// 设置顶点缓冲描述
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 指向保存顶点数据的临时缓冲.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &m_vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 创建顶点缓冲.
	HRESULT result = p3DRoot->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 设置索引缓冲描述.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 指向存临时索引缓冲.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &m_indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 创建索引缓冲.
	result = p3DRoot->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void CMtlShape::Shutdown()
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
}

bool CMtlShape::BindRender(CD3D11Class* p3DRoot)
{
	// 设置顶点缓冲跨度和偏移.
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	//在input assemberl阶段绑定顶点缓冲，以便能够被渲染
	p3DRoot->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//在input assemberl阶段绑定索引缓冲，以便能够被渲染
	p3DRoot->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 设置体元语义，渲染三角形列表.
	p3DRoot->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

D3DXMATRIX CMtlShape::GetWorldMatrix() const
{
	return m_mxWorld;
}

std::wstring CMtlShape::GetMtlName() const
{
	return m_strMtlName;
}

std::wstring CMtlShape::GetShaderName() const
{
	return m_strShaderName;
}

UINT CMtlShape::GetIndexCount() const
{
	return m_indices.size();
}
