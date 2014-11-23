#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include "textureclass.h"

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	ModelClass(void);
	ModelClass(const ModelClass &);

	~ModelClass(void);
	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	//便于在shader类调用，返回纹理资源
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	//顶点缓冲和顶点索引缓冲
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;
};

