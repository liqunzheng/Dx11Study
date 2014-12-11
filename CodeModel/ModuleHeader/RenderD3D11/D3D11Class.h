#pragma once
#include "RenderExport.h"
#include "AZGeometry.h"
#include "Camera.h"

class CShaderManager;
class CTextureManager;
class CShapeManager;
class CFontTextureManager;

class RENDERD3D11_API CD3D11Class
{
public:
	CD3D11Class();
	~CD3D11Class();

	bool Initialize(bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(float red, float green, float blue, float alpha);
	void Render();
	void EndScene();

	void TurnZBufferOn();
	void TrueZBufferOff();

	ID3D11Device*			GetDevice();
	ID3D11DeviceContext*	GetDeviceContext();
	const std::wstring&		GetVideoCardInfo(int& memory) const;
	const GeMatrix&			GetProjectionMatrix() const;
	const GeMatrix&			GetOrthoMatrix() const;
	CShaderManager*			getShaderMgr();
	CTextureManager*		getTextureMgr();
	CShapeManager*			getModelMgr();
	CFontTextureManager*	getFontMgr();

	GeMatrix getViewMatrix();
	CCamera* getCamera();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	void getViewSize(int& iWidth, int& iHeight);

private:
	bool m_vsync_enabled; //是否启用垂直同步
	int	 m_videoCardMemory; //显存大小
	std::wstring m_strCardDescription; //显卡名字

	IDXGISwapChain*				m_swapChain; //交换链对象
	ID3D11Device*				m_device;  //设备对象
	ID3D11DeviceContext*		m_deviceContext; //设备上下文对象
	ID3D11RenderTargetView*		m_renderTargetView; //渲染目标视图
	ID3D11Texture2D*			m_depthStencilBuffer;
	ID3D11DepthStencilState*	m_depthStencilState; //深度测试开启状态
	ID3D11DepthStencilState*    m_depthDisabledStencilState; //深度测试关闭状态
	ID3D11DepthStencilView*		m_depthStencilView; //深度目标视图
	ID3D11RasterizerState*		m_rasterState; //渲染状态

	GeMatrix m_projectionMatrix;//透视投影矩阵
	GeMatrix m_orthoMatrix;		//正交投影矩阵

	CCamera m_camera; //相机
	GeMatrix m_viewMatrix; //相机矩阵


	CShaderManager* m_shaderMgr;
	CTextureManager* m_textureMgr;
	CShapeManager* m_shapeMgr;
	CFontTextureManager *m_fontTexMgr;

	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
};

