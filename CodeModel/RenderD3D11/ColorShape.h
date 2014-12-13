/*************************************************
// ����: ��Ⱥ��  2014/12/13
// ����: ֻ�ж������ɫ��ʵ��
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
	//����
	std::vector<pt_xyz_c> m_vertices;

	//����
	CIndexArray	m_indices;

	//�������
	D3DXMATRIX m_mxWorld;

	//���㻺��
	ID3D11Buffer *m_vertexBuffer;

	//��������
	ID3D11Buffer *m_indexBuffer;
};

