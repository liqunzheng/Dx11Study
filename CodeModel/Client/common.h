#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

#if defined(DEBUG) || defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <dxerr.h>
#include <cassert>
#include "TraceWin.h"

//一个简单的错误检测宏

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
	{                                                          \
	HRESULT hr = (x);                                      \
if (FAILED(hr))                                         \
		{                                                      \
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

//定义一些常用颜色
const D3DXVECTOR4 WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXVECTOR4 BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXVECTOR4 RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXVECTOR4 GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXVECTOR4 BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const D3DXVECTOR4 YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const D3DXVECTOR4 CYAN(0.0f, 1.0f, 1.0f, 1.0f); //蓝绿色
const D3DXVECTOR4 MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);//洋红色

const D3DXVECTOR4 BEACH_SAND(1.0f, 0.96f, 0.62f, 1.0f);
const D3DXVECTOR4 LIGHT_YELLOW_GREEN(0.48f, 0.77f, 0.46f, 1.0f);
const D3DXVECTOR4 DARK_YELLOW_GREEN(0.1f, 0.48f, 0.19f, 1.0f);
const D3DXVECTOR4 DARKBROWN(0.45f, 0.39f, 0.34f, 1.0f);

struct sRay
{
	D3DXVECTOR3 origin;
	D3DXVECTOR3 direction;
};


class cBoundingBox
{
public:
	D3DXVECTOR3 m_min, m_max;

	cBoundingBox()
	{
		m_min.x = FLT_MAX;
		m_min.y = FLT_MAX;
		m_min.z = FLT_MAX;

		m_max.x = -FLT_MAX;
		m_max.y = -FLT_MAX;
		m_max.z = -FLT_MAX;
	}
	bool is_point_inside(D3DXVECTOR3& point)
	{
		return (point.x >= m_min.x && point.y >= m_min.y && point.z >= m_min.z &&
			point.x <= m_max.x && point.y <= m_max.y && point.z <= m_max.z);
	}
};

class cBoundingSphere
{
public:
	D3DXVECTOR3 m_center;
	float		m_radius;

	cBoundingSphere()
	{
		m_radius = 0.0f;
	}
};