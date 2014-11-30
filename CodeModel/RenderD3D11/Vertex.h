//*******************************************************************************
// ���ܣ���ӳ��ö����ʽ
// ---------------
// ���ߣ���Ⱥ��
// ʱ�䣺2014��11��30��
// xyz	��ά����
// xy	��ά����
// c	������ɫ
// uv	��������
//*******************************************************************************
#pragma once

struct  pt_xyz_c_uv
{
	D3DXVECTOR3 position;
	D3DXVECTOR4 color;
	D3DXVECTOR2 texture;
};

struct  pt_xyz_uv
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
};

struct pt_xyz_c
{
	D3DXVECTOR3 position;
	D3DXVECTOR4 color;
};

typedef std::vector<unsigned int> CIndexArray;


//////////////////////////////////////////////////////////////////////////
//��������
//������������
ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pDevice, const CIndexArray& indices);

template<class pt>
ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pDevice, const std::vector<pt>& vertices)
{
	// ���ö��㻺������
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(pt)* vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// ָ�򱣴涥�����ݵ���ʱ����.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// �������㻺��.
	ID3D11Buffer *pBuffer = nullptr;
	HRESULT result = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &pBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}
	return pBuffer;
}

