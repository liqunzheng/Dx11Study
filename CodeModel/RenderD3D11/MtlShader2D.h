#pragma once
#include "IShader.h"

class CMtlShader2D : public IShader
{
public:
	CMtlShader2D();
	~CMtlShader2D();

	virtual bool Initialize(CD3D11Class* p3DRoot);
	virtual void Shutdown();
	virtual bool Render(CD3D11Class* p3DRoot, UINT indexCount);
	virtual bool SetMatrix(CD3D11Class* p3DRoot, D3DXMATRIX mxWorld, D3DXMATRIX mxView, D3DXMATRIX mxProject);
	virtual bool SetShaderResource(CD3D11Class* p3DRoot, ID3D11ShaderResourceView* pResource);

protected:
	std::wstring m_strName;
	std::wstring m_strPSName;
	std::wstring m_strVSName;

	ID3D11VertexShader	*m_vertexShader;
	ID3D11PixelShader	*m_pixelShader;
	ID3D11InputLayout	*m_layout; //顶点输入布局
	ID3D11Buffer		*m_matrixBuffer; //变换矩阵
	ID3D11SamplerState	*m_sampleState;//采样状态
};

