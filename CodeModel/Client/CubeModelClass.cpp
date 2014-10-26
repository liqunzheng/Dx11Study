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

	//���ȣ����Ǵ���2����ʱ�����Ŷ�����������ݣ��Ա����ʹ�á�. 

	// ���ö��㻺���СΪ8��һ��������.
	m_vertexCount = 8;

	// �������������С.
	m_indexCount = 36;

	// ����������ʱ����.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// ������������.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}
	//����˳ʱ�뷽��������Σ����ֹ���
	// ���ö�������.
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

	// ����������������.
	indices[0] = 0;  // ǰ��
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	indices[6] = 4;  // ����
	indices[7] = 6;
	indices[8] = 5;
	indices[9] = 4;
	indices[10] = 7;
	indices[11] = 6;

	indices[12] = 4;  // ����
	indices[13] = 5;
	indices[14] = 1;
	indices[15] = 4;
	indices[16] = 1;
	indices[17] = 0;

	indices[18] = 3;  //����
	indices[19] = 2;
	indices[20] = 6;
	indices[21] = 3;
	indices[22] = 6;
	indices[23] = 7;

	indices[24] = 1;  // ����
	indices[25] = 5;
	indices[26] = 6;
	indices[27] = 1;
	indices[28] = 6;
	indices[29] = 2;

	indices[30] = 4;  // ����
	indices[31] = 0;
	indices[32] = 3;
	indices[33] = 4;
	indices[34] = 3;
	indices[35] = 7;


	// ���ö��㻺������
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// ָ�򱣴涥�����ݵ���ʱ����.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// �������㻺��.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	// ����������������.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ָ�����ʱ��������.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// ������������.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	D3DXComputeBoundingSphere((D3DXVECTOR3*)vertices, sizeof(vertices), sizeof(VertexType), &m_boundingSphere.m_center, &m_boundingSphere.m_radius);


	// �ͷ���ʱ����.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void CubeModelClass::ShutdownBuffers()
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

	return;
}

void CubeModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// ���ö��㻺���Ⱥ�ƫ��.
	stride = sizeof(VertexType);
	offset = 0;

	//��input assemberl�׶ΰ󶨶��㻺�壬�Ա��ܹ�����Ⱦ
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//��input assemberl�׶ΰ��������壬�Ա��ܹ�����Ⱦ
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ������Ԫ���壬��Ⱦ�������б�.
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
