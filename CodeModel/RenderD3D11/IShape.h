#pragma once

class CD3D11Class;

class IShape
{
public:
	virtual bool Initialize(CD3D11Class*) = 0;
	virtual void Shutdown() = 0;
	virtual bool BindRender(CD3D11Class*) = 0;
	virtual D3DXMATRIX GetWorldMatrix() const = 0;
	virtual const std::wstring& GetMtlName() const = 0;
	virtual const std::wstring& GetShaderName() const = 0;
	virtual UINT GetIndexCount() const = 0;
};
