#pragma once
#include "LineShape.h"

class CAxisShape : public CLineShape
{
public:
	CAxisShape();
	~CAxisShape();
	virtual bool BindRender(CD3D11Class* p3DRoot);
};

