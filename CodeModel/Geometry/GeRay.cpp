#include "stdafx.h"
#include "GeRay.h"


GeRay::GeRay()
{
}


GeRay::~GeRay()
{
}

const GePoint3& GeRay::getOrigin() const
{
	return m_ptOrigin;
}

void GeRay::setOrigin(const GePoint3& pt)
{
	m_ptOrigin = pt;
}

const GeVector3& GeRay::getDirection() const
{
	return m_ptDirection;
}

void GeRay::setDirection(const GeVector3& pt)
{
	m_ptDirection = pt;
}
