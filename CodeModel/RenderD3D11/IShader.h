#pragma once

class CD3D11Class;

class IShader
{
public:
	virtual bool Initialize(CD3D11Class*) = 0;
	virtual void Shutdown() = 0;
	virtual bool Render(CD3D11Class*, UINT) = 0;
	virtual bool SetMatrix(CD3D11Class*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX) = 0;
	virtual bool SetShaderResource(CD3D11Class*, ID3D11ShaderResourceView*) = 0;
};
