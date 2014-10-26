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
	//和shader中const buffer结构一致，主要用来给这些shader参数赋值。
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct  LightMaterialBufferType
	{
		D3DXVECTOR4 lightPosition; //光源位置
		D3DXVECTOR4 lightColor;   //光源颜色
		D3DXVECTOR4 globalAmbient; //光源的环境光反射系数
		D3DXVECTOR4 cameraPosition; //摄像机的位置
		D3DXVECTOR4 Ke;  //材质的自发光
		D3DXVECTOR4 Ka;  //材质的环境光系数
		D3DXVECTOR4 Kd;  //材质的漫反射系数
		D3DXVECTOR4 Ks;  //材质的高光系数
		D3DXVECTOR3 lightDirection; //平行光方向
		float shininess; //高光指数

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
	ID3D11Buffer* m_lightmaterialBuffer; //材质光照参数buffer
};

