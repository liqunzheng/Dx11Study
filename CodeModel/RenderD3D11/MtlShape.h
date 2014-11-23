#pragma once
#include "IShape.h"


class CMtlShape : public IShape
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	CMtlShape();
	~CMtlShape();

	virtual bool Initialize(CD3D11Class* p3DRoot);
	virtual void Shutdown();
	virtual bool BindRender(CD3D11Class* p3DRoot);

	virtual D3DXMATRIX GetWorldMatrix() const;
	virtual const std::wstring& GetMtlName() const;
	virtual const std::wstring& GetShaderName() const;
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

