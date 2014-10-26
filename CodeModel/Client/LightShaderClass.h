#pragma once
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
#include "common.h"
using namespace std;

class LightShaderClass
{
private:
	//��shader��const buffer�ṹһ�£���Ҫ��������Щshader������ֵ��
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct  LightMaterialBufferType
	{
		D3DXVECTOR4 lightPosition; //��Դλ��
		D3DXVECTOR4 lightColor;   //��Դ��ɫ
		D3DXVECTOR4 globalAmbient; //��Դ�Ļ����ⷴ��ϵ��
		D3DXVECTOR4 cameraPosition; //�������λ��
		D3DXVECTOR4 Ke;  //���ʵ��Է���
		D3DXVECTOR4 Ka;  //���ʵĻ�����ϵ��
		D3DXVECTOR4 Kd;  //���ʵ�������ϵ��
		D3DXVECTOR4 Ks;  //���ʵĸ߹�ϵ��
		D3DXVECTOR3 lightDirection; //ƽ�йⷽ��
		float shininess; //�߹�ָ��

	};
public:
	LightShaderClass(void);
	LightShaderClass(const LightShaderClass &);
	~LightShaderClass(void);
	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4,
		D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, float);
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4,
		D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, float);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightmaterialBuffer; //���ʹ��ղ���buffer
};

