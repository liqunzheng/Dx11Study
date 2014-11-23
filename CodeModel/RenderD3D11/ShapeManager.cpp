#include "stdafx.h"
#include "D3D11Class.h"
#include "ShapeManager.h"
#include "MtlShape.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "IShader.h"



CShapeManager::CShapeManager()
{
}


CShapeManager::~CShapeManager()
{
	Shutdown();
}

void CShapeManager::Initialize(const std::wstring& strFileName, CD3D11Class* pD3DRoot)
{
	CMtlShape *pModel = new CMtlShape();
	pModel->Initialize(pD3DRoot);
	m_models.push_back(pModel);	
}

void CShapeManager::Shutdown()
{
	for (size_t i = 0; i < m_models.size(); ++i)
	{
		m_models[i]->Shutdown();
		delete m_models[i];
		m_models[i] = nullptr;
	}

	m_models.clear();
}

void CShapeManager::Render(CD3D11Class* pD3DRoot)
{
	for (size_t i = 0; i < m_models.size(); ++i)
	{
		IShape* pModel = m_models[i];
		pModel->BindRender(pD3DRoot);

		ID3D11ShaderResourceView* pTexture = pD3DRoot->getTextureMgr()->GetTexture(pModel->GetMtlName(), pD3DRoot);
		IShader* pShader = pD3DRoot->getShaderMgr()->getShader(pModel->GetShaderName(), pD3DRoot);
		D3DXMATRIX mxView = pD3DRoot->getViewMatrix();
		D3DXMATRIX mxProject = pD3DRoot->GetProjectionMatrix();
		pShader->SetMatrix(pD3DRoot, pModel->GetWorldMatrix(), mxView, mxProject);
		pShader->SetShaderResource(pD3DRoot, pTexture);
		pShader->Render(pD3DRoot, pModel->GetIndexCount());
	}
}
