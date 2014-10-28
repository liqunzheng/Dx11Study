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
	CameraClass* m_Camera;

	void pick(int x, int y);
private:
	bool Render();

	//定义一个D3DClass类成员变量
	D3DClass* m_D3D;
	D2DClass* m_D2D;

	ModelClass* m_Model;
	AxisModelClass* m_AxisModel;
	ColorShaderClass* m_ColorShader;

	PlaneModelClass* m_PlaneModel;
	CubeModelClass* m_CubeModel;

	LightClass* m_Light;
	LightShaderClass *m_LightShader;
};

