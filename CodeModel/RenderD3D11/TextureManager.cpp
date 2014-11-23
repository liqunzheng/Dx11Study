#include "stdafx.h"
#include "TextureManager.h"
#include "D3DX11tex.h"
#include "D3D11Class.h"


CTextureManager::CTextureManager()
{
}


CTextureManager::~CTextureManager()
{
	Shutdown();
}

ID3D11ShaderResourceView* CTextureManager::GetTexture(const std::wstring& strMtlName, CD3D11Class* pD3DRoot)
{
	auto iter = m_mapTexture.find(strMtlName);
	if (iter == m_mapTexture.end())
	{
		ID3D11Device* device = pD3DRoot->GetDevice();
		ID3D11ShaderResourceView* pTexture;
		HRESULT result = D3DX11CreateShaderResourceViewFromFile(device, strMtlName.c_str(), NULL, NULL, &pTexture, NULL);
		if (FAILED(result))
		{
			return nullptr;
		}
		iter = m_mapTexture.insert(std::make_pair(strMtlName, pTexture)).first;
	}

	return iter->second;
}

void CTextureManager::Shutdown()
{
	for (auto iter = m_mapTexture.begin(); iter != m_mapTexture.end(); ++iter)
	{
		iter->second->Release();
		iter->second = NULL;
	}

	m_mapTexture.clear();
}
