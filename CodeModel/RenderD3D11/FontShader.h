#pragma once
#include "IShader.h"
class CFontShader : public IShader
{
	//��shader��const buffer�ṹһ�£���Ҫ��������Щshader������ֵ��
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
	ID3D11InputLayout	*m_layout; //�������벼��
	ID3D11Buffer		*m_matrixBuffer; //�任����
	ID3D11SamplerState	*m_sampleState;//����״̬
};

