#pragma once
#include "AZGeometry.h"

class GEOMETRY_API GeRay
{
public:
	GeRay();
	~GeRay();

	const D3DXVECTOR3& getOrigin() const;
	void setOrigin(const D3DXVECTOR3& pt);

	const D3DXVECTOR3& getDirection() const;
	void setDirection(const D3DXVECTOR3& pt);

private:
	D3DXVECTOR3 m_ptOrigin; //起点
	D3DXVECTOR3 m_ptDirection; //方向
};

