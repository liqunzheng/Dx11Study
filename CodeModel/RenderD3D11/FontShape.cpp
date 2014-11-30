#include "stdafx.h"
#include "FontShape.h"
#include "D3D11Class.h"
#include "FontTextureManager.h"

CFontShape::CFontShape()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer= nullptr;
}

CFontShape::~CFontShape()
{
	Shutdown();
}

bool CFontShape::Initialize(CD3D11Class* p3DRoot)
{
	D3DXMatrixIdentity(&m_mxWorld);
	CFontTextureManager* pFtMgr = p3DRoot->getFontMgr();
	std::string strInfo = "我有一个梦想！";
	RECT rect = { 0, 0, 0, 0 };
	D3DXVECTOR4 clr = { 1.0f, 0.0f, 0.0f, 1.0f };
	pFtMgr->CalTextData(strInfo, 32, rect, clr, m_vertices, m_indices);
	m_vertexBuffer = CreateVertexBuffer(p3DRoot->GetDevice(), m_vertices);
	m_indexBuffer = CreateIndexBuffer(p3DRoot->GetDevice(), m_indices);
	assert(m_vertexBuffer && m_indexBuffer);
	return true;
}

void CFontShape::Shutdown()
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

bool CFontShape::BindRender(CD3D11Class* p3DRoot)
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
	UINT stride = sizeof(pt_xyz_c_uv);
	UINT offset = 0;

	//在input assemberl阶段绑定顶点缓冲，以便能够被渲染
	p3DRoot->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//在input assemberl阶段绑定索引缓冲，以便能够被渲染
	p3DRoot->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 设置体元语义，渲染三角形列表.
	p3DRoot->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

D3DXMATRIX CFontShape::GetWorldMatrix() const
{
	return m_mxWorld;
}

std::wstring CFontShape::GetMtlName() const
{
	return L"uFont";
}

std::wstring CFontShape::GetShaderName() const
{
	return L"CFontShader";
}

UINT CFontShape::GetIndexCount() const
{
	return m_indices.size();
}

