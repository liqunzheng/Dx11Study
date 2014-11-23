#include "stdafx.h"
#include "ShaderManager.h"
#include "MtlShader.h"


CShaderManager::CShaderManager()
{
}


CShaderManager::~CShaderManager()
{
	Shutdown();
}

void CShaderManager::Shutdown()
{
	for (auto iter = m_mapShader.begin(); iter != m_mapShader.end(); ++iter)
	{
		iter->second->Shutdown();
		delete iter->second;
		iter->second = nullptr;
	}
	m_mapShader.clear();
}

IShader* CShaderManager::getShader(const std::wstring& strShaderName, CD3D11Class* pD3DRoot)
{
	auto iter = m_mapShader.find(strShaderName);
	if (iter == m_mapShader.end())
	{
		IShader* pShader = createShader(strShaderName, pD3DRoot);
		if (pShader == nullptr)
		{
			return nullptr;
		}
		iter = m_mapShader.insert(std::make_pair(strShaderName, pShader)).first;
	}

	return iter->second;
}

IShader* CShaderManager::createShader(const std::wstring& strShaderName, CD3D11Class* pD3DRoot)
{
	IShader *pShader = nullptr;
	if (strShaderName.compare(L"CMtlShader") == 0)
	{
		pShader = new CMtlShader();
		pShader->Initialize(pD3DRoot);
	}
	return pShader;
}
