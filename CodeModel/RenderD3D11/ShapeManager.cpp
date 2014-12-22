#include "stdafx.h"
#include "D3D11Class.h"
#include "ShapeManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "IShader.h"
#include "MtlShape.h"
#include "LineShape.h"
#include "AxisShape.h"
#include "FontShape.h"
#include "MtlShape2D.h"
#include "LoadShape.h"


CShapeManager::CShapeManager()
{
}


CShapeManager::~CShapeManager()
{
	Shutdown();
}

void CShapeManager::Initialize(const std::wstring& strFileName, CD3D11Class* pD3DRoot)
{
	//äÖÈ¾×ø±êÖá
	CAxisShape *pAxis = new CAxisShape();
	pAxis->Initialize(pD3DRoot);
	m_models.push_back(pAxis);

	std::vector<IShape*> pShapes = CLoadShape::Instance()->load(this, L"aaa.msh");

	for (size_t i = 0; i < pShapes.size(); ++i)
	{
		pShapes[i]->Initialize(pD3DRoot);
		m_models.push_back(pShapes[i]);
	}
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

		IShader* pShader = pD3DRoot->getShaderMgr()->getShader(pModel->GetShaderName(), pD3DRoot);
		D3DXMATRIX mxView = pD3DRoot->getViewMatrix();
		D3DXMATRIX mxProject = pD3DRoot->GetProjectionMatrix();
		pShader->SetMatrix(pD3DRoot, pModel->GetWorldMatrix(), mxView, mxProject);

		ID3D11ShaderResourceView* pTexture = pD3DRoot->getTextureMgr()->GetTexture(pModel->GetMtlName(), pD3DRoot);
		pShader->SetShaderResource(pD3DRoot, pTexture);

		pShader->Render(pD3DRoot, pModel->GetIndexCount());
	}
}
