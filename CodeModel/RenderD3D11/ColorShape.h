/*************************************************
// 作者: 李群正  2014/12/13
// 描述: 只有顶点和颜色的实体
//*************************************************/
#pragma once

#include "IShape.h"

class CColorShape : public IShape
{
	friend class CLoadShape;
public:
	CColorShape();
	~CColorShape();

	virtual bool Initialize(CD3D11Class*) override;

	virtual void Shutdown() override;

	virtual bool BindRender(CD3D11Class*) override;

	virtual D3DXMATRIX GetWorldMatrix() const override;

	virtual std::wstring GetMtlName() const override;

	virtual std::wstring GetShaderName() const override;

	virtual UINT GetIndexCount() const override;

private:
	//顶点
	std::vector<pt_xyz_c> m_vertices;

	//索引
	CIndexArray	m_indices;

	//世界矩阵
	D3DXMATRIX m_mxWorld;

	//顶点缓冲
	ID3D11Buffer *m_vertexBuffer;

	//索引缓冲
	ID3D11Buffer *m_indexBuffer;
};

