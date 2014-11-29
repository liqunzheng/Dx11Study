#pragma once

class CD3D11Class;

class CTextureManager
{
public:
	CTextureManager();
	~CTextureManager();

	ID3D11ShaderResourceView* GetTexture(const std::wstring& strMtlName, CD3D11Class* pD3DRoot);
	void Shutdown();
	void add(const std::wstring& strMtlName, ID3D11ShaderResourceView* pTexture);
private:
	std::map<std::wstring, ID3D11ShaderResourceView*> m_mapTexture;
};

