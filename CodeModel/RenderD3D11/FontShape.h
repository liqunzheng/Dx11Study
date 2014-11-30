//*******************************************************************************
// 功能：绘制文字
// ---------------
// 作者：李群政
// 时间：2014年11月30日
//*******************************************************************************
#pragma once
#include "IShape.h"

class CFontShape : public IShape
{
public:
	CFontShape();
	~CFontShape();

	virtual bool Initialize(CD3D11Class*) override;

	virtual void Shutdown() override;

	virtual bool BindRender(CD3D11Class*) override;

	virtual D3DXMATRIX GetWorldMatrix() const override;

	virtual std::wstring GetMtlName() const override;

	virtual std::wstring GetShaderName() const override;

	virtual UINT GetIndexCount() const override;

protected:
	std::vector<pt_xyz_c_uv> m_vertices;
	CIndexArray m_indices;

	D3DXMATRIX				m_mxWorld;

	//顶点缓冲
	ID3D11Buffer *m_vertexBuffer;
	//索引缓冲
	ID3D11Buffer *m_indexBuffer;

};

