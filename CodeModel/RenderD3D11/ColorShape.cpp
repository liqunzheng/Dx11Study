#include "stdafx.h"
#include "ColorShape.h"
#include "D3D11Class.h"


CColorShape::CColorShape()
{
	D3DXMatrixIdentity(&m_mxWorld);
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
}


CColorShape::~CColorShape()
{
	Shutdown();
}

bool CColorShape::Initialize(CD3D11Class* p3dRoot)
{
	m_vertexBuffer = CreateVertexBuffer(p3dRoot->GetDevice(), m_vertices);
	m_indexBuffer = CreateIndexBuffer(p3dRoot->GetDevice(), m_indices);
	return true;
}

void CColorShape::Shutdown()
{
	// �ͷŶ��㻺��.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// �ͷ���������
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

bool CColorShape::BindRender(CD3D11Class* p3dRoot)
{
	// ���ö��㻺���Ⱥ�ƫ��.
	UINT stride = sizeof(pt_xyz_c);
	UINT offset = 0;

	//��input assemberl�׶ΰ󶨶��㻺�壬�Ա��ܹ�����Ⱦ
	p3dRoot->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//��input assemberl�׶ΰ��������壬�Ա��ܹ�����Ⱦ
	p3dRoot->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ������Ԫ���壬��Ⱦ�������б�.
	p3dRoot->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

D3DXMATRIX CColorShape::GetWorldMatrix() const
{
	return m_mxWorld;
}

std::wstring CColorShape::GetMtlName() const
{
	return L"";
}

std::wstring CColorShape::GetShaderName() const
{
	return L"CColorShader";
}

UINT CColorShape::GetIndexCount() const
{
	return m_indices.size();
}
