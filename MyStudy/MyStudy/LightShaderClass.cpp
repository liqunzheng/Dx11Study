#include "LightShaderClass.h"


LightShaderClass::LightShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_lightmaterialBuffer = 0;
}


LightShaderClass::~LightShaderClass()
{
}

bool LightShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//≥ı ºªØvs∫Õps
	result = InitializeShader(device, hwnd, L"light.vs", L"light.ps");
	if (!result)
	{
		return false;
	}
	return true;
}

void LightShaderClass::Shutdown()
{
	ShutdownShader();
}

bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix, D3DXVECTOR3 cameraPosition)
{
	bool result;
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, cameraPosition);
	if (!result)
	{
		HR(result);
		return false;
	}

	RenderShader(deviceContext, indexCount);
	return true;
}

bool LightShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	return false;
}

void LightShaderClass::ShutdownShader()
{

}

void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*)
{

}

bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR3)
{
	return false;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext*, int)
{

}

