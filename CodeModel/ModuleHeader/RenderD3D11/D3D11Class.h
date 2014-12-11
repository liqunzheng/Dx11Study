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
	bool m_vsync_enabled; //�Ƿ����ô�ֱͬ��
	int	 m_videoCardMemory; //�Դ��С
	std::wstring m_strCardDescription; //�Կ�����

	IDXGISwapChain*				m_swapChain; //����������
	ID3D11Device*				m_device;  //�豸����
	ID3D11DeviceContext*		m_deviceContext; //�豸�����Ķ���
	ID3D11RenderTargetView*		m_renderTargetView; //��ȾĿ����ͼ
	ID3D11Texture2D*			m_depthStencilBuffer;
	ID3D11DepthStencilState*	m_depthStencilState; //��Ȳ��Կ���״̬
	ID3D11DepthStencilState*    m_depthDisabledStencilState; //��Ȳ��Թر�״̬
	ID3D11DepthStencilView*		m_depthStencilView; //���Ŀ����ͼ
	ID3D11RasterizerState*		m_rasterState; //��Ⱦ״̬

	GeMatrix m_projectionMatrix;//͸��ͶӰ����
	GeMatrix m_orthoMatrix;		//����ͶӰ����

	CCamera m_camera; //���
	GeMatrix m_viewMatrix; //�������


	CShaderManager* m_shaderMgr;
	CTextureManager* m_textureMgr;
	CShapeManager* m_shapeMgr;
	CFontTextureManager *m_fontTexMgr;

	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
};

