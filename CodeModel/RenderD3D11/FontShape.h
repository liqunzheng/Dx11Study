//*******************************************************************************
// ���ܣ���������
// ---------------
// ���ߣ���Ⱥ��
// ʱ�䣺2014��11��30��
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

	//���㻺��
	ID3D11Buffer *m_vertexBuffer;
	//��������
	ID3D11Buffer *m_indexBuffer;

};

