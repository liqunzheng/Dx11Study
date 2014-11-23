#pragma once
#include "IShader.h"

class CMtlShader : public IShader
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
	CMtlShader();
	~CMtlShader();

	virtual bool Initialize(CD3D11Class* pD3DClass);
	virtual void Shutdown();
	virtual bool Render(CD3D11Class* pD3DClass, UINT indexCount);
	virtual bool SetMatrix(CD3D11Class* pD3DClass, D3DXMATRIX mxWorld, D3DXMATRIX mxView, D3DXMATRIX mxProject);
	virtual bool SetShaderResource(CD3D11Class* pD3DClass, ID3D11ShaderResourceView* pResource);

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

