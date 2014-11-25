#include "stdafx.h"
#include "LineShape.h"
#include "D3D11Class.h"

CLineShape::CLineShape()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	D3DXMatrixIdentity(&m_mxWorld);
}


CLineShape::~CLineShape()
{
	Shutdown();
}

bool CLineShape::Initialize(CD3D11Class* p3DRoot)
{
	m_vertices.resize(6);
	m_indices.resize(6);

	//x�� ��ɫ
	m_vertices[0].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vertices[0].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	m_vertices[1].position = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	m_vertices[1].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	//y�ᣬ��ɫ 
	m_vertices[2].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	m_vertices[3].position = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	m_vertices[3].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	//z�ᣬ��ɫ 
	m_vertices[4].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vertices[4].color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	m_vertices[5].position = D3DXVECTOR3(0.0f, 0.0f, 10.0f);
	m_vertices[5].color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	for (size_t i = 0; i < m_indices.size(); ++i)
	{
		m_indices[i] = i;
	}

	// ���ö��㻺������
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// ָ�򱣴涥�����ݵ���ʱ����.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &m_vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// �������㻺��.
	HRESULT result = p3DRoot->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// ����������������.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ָ�����ʱ��������.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &m_indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// ������������.
	result = p3DRoot->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void CLineShape::Shutdown()
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

bool CLineShape::BindRender(CD3D11Class* p3DRoot)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	//��input assemberl�׶ΰ󶨶��㻺�壬�Ա��ܹ�����Ⱦ
	p3DRoot->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//��input assemberl�׶ΰ��������壬�Ա��ܹ�����Ⱦ
	p3DRoot->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ������Ԫ���壬��Ⱦ�߶�
	p3DRoot->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	
	return true;
}

D3DXMATRIX CLineShape::GetWorldMatrix() const
{
	return m_mxWorld;
}

std::wstring CLineShape::GetMtlName() const
{
	return L"";
}

std::wstring CLineShape::GetShaderName() const
{
	return L"CColorShader";
}

UINT CLineShape::GetIndexCount() const
{
	return m_indices.size();
}
