#pragma once
#include "IShape.h"
//////////////////////////////////////////////////////////////////////////
//��ά���ƣ���Ļ���½���0.0��

class CMtlShape2D : public IShape
{
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	CMtlShape2D();
	~CMtlShape2D();

	virtual bool Initialize(CD3D11Class*);

	virtual void Shutdown();

	virtual bool BindRender(CD3D11Class*);

	virtual D3DXMATRIX GetWorldMatrix() const;

	virtual std::wstring GetMtlName() const;

	virtual std::wstring GetShaderName() const;

	virtual UINT GetIndexCount() const;

protected:
	std::vector<VertexType> m_vertices;
	std::vector<int>		m_indices;
	std::wstring			m_strMtlName;
	std::wstring			m_strShaderName;

	D3DXMATRIX				m_mxWorld;

	//���㻺��
	ID3D11Buffer *m_vertexBuffer;
	//��������
	ID3D11Buffer *m_indexBuffer;
};

