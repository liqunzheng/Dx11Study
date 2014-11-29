#include "stdafx.h"
#include "MtlShape2D.h"
#include "D3D11Class.h"


CMtlShape2D::CMtlShape2D()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


CMtlShape2D::~CMtlShape2D()
{
	Shutdown();
}

bool CMtlShape2D::Initialize(CD3D11Class* p3DRoot)
{
	m_strShaderName = L"CMtlShader";
	m_strMtlName = L"tong.dds";
	D3DXMatrixIdentity(&m_mxWorld);

	m_indices.resize(6);
	m_vertices.resize(4);

	//创建顺时针方向的三角形，左手规则
	// 设置顶点数据.
	m_vertices[0].position = D3DXVECTOR3(0.0f, 0.0f, 0.01f);
	m_vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);

	m_vertices[1].position = D3DXVECTOR3(0.0f, 512.0f, 0.01f);
	m_vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);

	m_vertices[2].position = D3DXVECTOR3(512.0f, 512.0f, 0.01f);
	m_vertices[2].texture = D3DXVECTOR2(1.0f, 0.0f);

	m_vertices[3].position = D3DXVECTOR3(512.0f, 0.0f, 0.01f);
	m_vertices[3].texture = D3DXVECTOR2(1.0f, 1.0f);

	// 设置索引缓冲数据.
	m_indices[0] = 1;
	m_indices[1] = 3;
	m_indices[2] = 0;
	m_indices[3] = 3;
	m_indices[4] = 1;
	m_indices[5] = 2;

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

void CMtlShape2D::Shutdown()
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

bool CMtlShape2D::BindRender(CD3D11Class* p3DRoot)
{
	int nWidth = 0;
	int nHeight = 0;

	p3DRoot->getViewSize(nWidth, nHeight);
	float fWidth = nWidth * 0.5f;
	float fHeight = nHeight*0.5f;
	D3DXMatrixIdentity(&m_mxWorld);

 	m_mxWorld._41 = -fWidth;
	m_mxWorld._42 = -fHeight;

	D3DXMATRIX mxScale;
	D3DXMatrixIdentity(&mxScale);
	mxScale._11 = 1.0f / fWidth;
	mxScale._22 = 1.0f / fHeight;
	D3DXMatrixMultiply(&m_mxWorld, &m_mxWorld, &mxScale);

	D3DXMATRIX mxInvView, mxInvProj;
	float fDet;
	D3DXMatrixInverse(&mxInvView, &fDet, &p3DRoot->getViewMatrix());
	D3DXMatrixInverse(&mxInvProj, &fDet, &p3DRoot->GetProjectionMatrix());

	D3DXMatrixMultiply(&m_mxWorld, &m_mxWorld, &mxInvProj);
	D3DXMatrixMultiply(&m_mxWorld, &m_mxWorld, &mxInvView);

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

D3DXMATRIX CMtlShape2D::GetWorldMatrix() const
{
	return m_mxWorld;
}

std::wstring CMtlShape2D::GetMtlName() const
{
	return m_strMtlName;
}

std::wstring CMtlShape2D::GetShaderName() const
{
	return m_strShaderName;
}

UINT CMtlShape2D::GetIndexCount() const
{
	return m_indices.size();
}
