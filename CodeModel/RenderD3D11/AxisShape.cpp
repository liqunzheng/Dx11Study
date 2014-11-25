#include "stdafx.h"
#include "AxisShape.h"
#include "D3D11Class.h"


CAxisShape::CAxisShape()
{
}


CAxisShape::~CAxisShape()
{
}

bool CAxisShape::BindRender(CD3D11Class* p3DRoot)
{
	D3DXMATRIX mxInvView, mxInvProj;
	float fDet;
	D3DXMatrixInverse(&mxInvView, &fDet, &p3DRoot->getViewMatrix());
	D3DXMatrixInverse(&mxInvProj, &fDet, &p3DRoot->GetProjectionMatrix());

	D3DXVECTOR3 v = { -0.9f, -0.9f, 0.5f };
	{
		D3DXVECTOR3 v1;
		D3DXVec3TransformCoord(&v1, &v, &mxInvProj);
		D3DXVec3TransformCoord(&v, &v1, &mxInvView);
	}

	float fScale = 1.0f;

	D3DXVECTOR3 vs = { -0.9f, -0.95f, 0.5f };
	{
		D3DXVECTOR3 v1;
		D3DXVec3TransformCoord(&v1, &vs, &mxInvProj);
		D3DXVec3TransformCoord(&vs, &v1, &mxInvView);
		v1 = v - vs;
		fScale = D3DXVec3Length(&v1) * 0.3f;
	}

	D3DXVECTOR3 vOld = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 vMove = v - vOld;
	D3DXMatrixIdentity(&m_mxWorld);
	m_mxWorld._41 = vMove.x;
	m_mxWorld._42 = vMove.y;
	m_mxWorld._43 = vMove.z;

	m_mxWorld._11 = fScale;
	m_mxWorld._22 = fScale;
	m_mxWorld._33 = fScale;

	return CLineShape::BindRender(p3DRoot);
}
