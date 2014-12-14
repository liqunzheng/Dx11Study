#include "stdafx.h"
#include "LightShader.h"


CLightShader::CLightShader()
{
	m_strName = L"CLightShader";
	m_strPSName = getShaderFile(L"light.ps");
	m_strVSName = getShaderFile(L"light.vs");

	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}


CLightShader::~CLightShader()
{
}

bool CLightShader::Initialize(CD3D11Class*)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void CLightShader::Shutdown()
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool CLightShader::Render(CD3D11Class*, UINT)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool CLightShader::SetMatrix(CD3D11Class*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool CLightShader::SetShaderResource(CD3D11Class*, ID3D11ShaderResourceView*)
{
	throw std::logic_error("The method or operation is not implemented.");
}
