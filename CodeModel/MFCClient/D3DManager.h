/*************************************************
// 作者: 李群正  2014/12/11
// 描述: 3D引擎管理类
//*************************************************/
#pragma once

class CD3D11Class;
class CCamera;

class CD3DManager
{
protected:
	CD3DManager();
	virtual ~CD3DManager();
public:
	//得到
	static CD3DManager* Instance();

	//初始化引擎
	bool InitEnger(HWND hwnd);

	//渲染
	void Render();

	// 得到当前相机
	CCamera* getCamera();
	
	//关闭
	void Shutdown();

protected:
	//定义一个D3DClass类成员变量
	CD3D11Class* m_p3dRoot;
};


