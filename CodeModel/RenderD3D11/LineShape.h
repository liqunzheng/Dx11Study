#pragma once
#include "IShape.h"

class CLineShape : public IShape
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

public:
	CLineShape();
	~CLineShape();

	virtual bool Initialize(CD3D11Class* p3DRoot);

	virtual void Shutdown();

	virtual bool BindRender(CD3D11Class* p3DRoot);

	virtual D3DXMATRIX GetWorldMatrix() const;

	virtual std::wstring GetMtlName() const;

	virtual std::wstring GetShaderName() const;

	virtual UINT GetIndexCount() const;

private:
	std::vector<VertexType> m_vertices;
	std::vector<int>		m_indices;
	D3DXMATRIX				m_mxWorld;

	//¶¥µã»º³å
	ID3D11Buffer *m_vertexBuffer;
	//Ë÷Òý»º³å
	ID3D11Buffer *m_indexBuffer;
};

