#pragma once

class CD3D11Class;
class IShape;


class CShapeManager
{
public:
	CShapeManager();
	~CShapeManager();

	void Initialize(const std::wstring& strFileName, CD3D11Class* pD3DRoot);
	void Shutdown();

	void Render(CD3D11Class* pD3DRoot);

protected:
	std::vector<IShape*> m_models;
};

