#pragma once
#include "IShader.h"

class CColorShader : public IShader
{
private:
	//��shader��const buffer�ṹһ�£���Ҫ��������Щshader������ֵ��
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

public:
	CColorShader();
	~CColorShader();

	virtual bool Initialize(CD3D11Class* p3DRoot);

	virtual void Shutdown();

	virtual bool Render(CD3D11Class* p3DRoot, UINT indexCount);

	virtual bool SetMatrix(CD3D11Class* p3DRoot, D3DXMATRIX mxWorld, D3DXMATRIX mxView, D3DXMATRIX mxProject);

	virtual bool SetShaderResource(CD3D11Class*, ID3D11ShaderResourceView*);

protected:
	std::wstring m_strName;
	std::wstring m_strPSName;
	std::wstring m_strVSName;

	ID3D11VertexShader	*m_vertexShader;
	ID3D11PixelShader	*m_pixelShader;
	ID3D11InputLayout	*m_layout; //�������벼��
	ID3D11Buffer		*m_matrixBuffer; //�任����
};

