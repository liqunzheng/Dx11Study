#include "stdafx.h"
#include "Vertex.h"


ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pDevice, const CIndexArray& indices)
{
	// ����������������.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int)* indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ָ�����ʱ��������.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// ������������.
	ID3D11Buffer* pBuffer = nullptr;
	HRESULT result = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &pBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}

	return pBuffer;
}
