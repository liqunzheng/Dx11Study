#include "stdafx.h"
#include "LightShape.h"


CLightShape::CLightShape()
{
}


CLightShape::~CLightShape()
{
}

bool CLightShape::Initialize(CD3D11Class*)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void CLightShape::Shutdown()
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool CLightShape::BindRender(CD3D11Class*)
{
	throw std::logic_error("The method or operation is not implemented.");
}

D3DXMATRIX CLightShape::GetWorldMatrix() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::wstring CLightShape::GetMtlName() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::wstring CLightShape::GetShaderName() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

UINT CLightShape::GetIndexCount() const
{
	throw std::logic_error("The method or operation is not implemented.");
}
