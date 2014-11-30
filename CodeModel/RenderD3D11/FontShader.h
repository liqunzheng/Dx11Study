#pragma once
#include "IShader.h"
class CFontShader : public IShader
{
	//和shader中const buffer结构一致，主要用来给这些shader参数赋值。
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
public:
	CFontShader();
	~CFontShader();

	virtual bool Initialize(CD3D11Class*);

	virtual void Shutdown();

	virtual bool Render(CD3D11Class*, UINT);

	virtual bool SetMatrix(CD3D11Class*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	virtual bool SetShaderResource(CD3D11Class*, ID3D11ShaderResourceView*);

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

