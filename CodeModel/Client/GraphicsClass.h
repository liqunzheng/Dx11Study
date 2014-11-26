#pragma once

#include <windows.h>
#include "common.h"

class CD3D11Class;
class CCamera;


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;//�Ƿ�ȫ��
const bool VSYNC_ENABLED = true;//�Ƿ�ֱͬ��
const float SCREEN_DEPTH = 1000.0f;//��ȣ�Զ��
const float SCREEN_NEAR = 0.1f;//��ȣ�����

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

	CCamera* getCamera();

	//����һ��D3DClass���Ա����
	CD3D11Class* m_p3dRoot;
};

