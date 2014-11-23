#pragma once

class IShader;
class CD3D11Class;

class CShaderManager
{
public:
	CShaderManager();
	~CShaderManager();

	IShader* getShader(const std::wstring& strShaderName, CD3D11Class* pD3DRoot);
	void Shutdown();

protected:
	std::map<std::wstring, IShader*> m_mapShader;
	IShader* createShader(const std::wstring& strShaderName, CD3D11Class* pD3DRoot);
};

