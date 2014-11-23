#pragma once

#include <windows.h>
#include "d3dclass.h"
#include "D2DClass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "AxisModelClass.h"
#include "colorshaderclass.h"
#include "CubeModelClass.h"
#include "PlaneModelClass.h"
#include "LightShaderClass.h"
#include "LightClass.h"
#include "D3D11Class.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;//是否全屏
const bool VSYNC_ENABLED = true;//是否垂直同步
const float SCREEN_DEPTH = 1000.0f;//深度，远点
const float SCREEN_NEAR = 0.1f;//深度，近点

class GraphicsClass
{
public:
	GraphicsClass(void);
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass(void);
	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	void pick(int x, int y);
	bool Render();

	//定义一个D3DClass类成员变量
	CD3D11Class* m_p3dRoot;
	CameraClass* m_Camera;
};

