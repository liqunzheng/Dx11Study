//*******************************************************************************
// 功能：添加常用顶点格式
// ---------------
// 作者：李群政
// 时间：2014年11月30日
// xyz	三维坐标
// xy	二维坐标
// c	顶点颜色
// uv	纹理坐标
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
//公共函数
//创建索引缓存
ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pDevice, const CIndexArray& indices);

template<class pt>
ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pDevice, const std::vector<pt>& vertices)
{
	// 设置顶点缓冲描述
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(pt)* vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 指向保存顶点数据的临时缓冲.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 创建顶点缓冲.
	ID3D11Buffer *pBuffer = nullptr;
	HRESULT result = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &pBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}
	return pBuffer;
}

