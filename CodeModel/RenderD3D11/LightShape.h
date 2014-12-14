#pragma once
#include "IShape.h"
#include "Vertex.h"

class CLightShape : public IShape
{
public:
	CLightShape();
	~CLightShape();

	virtual bool Initialize(CD3D11Class*);

	virtual void Shutdown();

	virtual bool BindRender(CD3D11Class*);

	virtual D3DXMATRIX GetWorldMatrix() const;

	virtual std::wstring GetMtlName() const;

	virtual std::wstring GetShaderName() const;

	virtual UINT GetIndexCount() const;

private:
	std::vector<pt_xyz_nxyz> m_vertices;
	CIndexArray m_indices;
	std::wstring			m_strMtlName;
	std::wstring			m_strShaderName;

	D3DXMATRIX				m_mxWorld;

	//¶¥µã»º³å
	ID3D11Buffer *m_vertexBuffer;
	//Ë÷Òý»º³å
	ID3D11Buffer *m_indexBuffer;
};

