#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include "common.h"

class PlaneModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal; //法向
	};
public:
	PlaneModelClass(void);
	PlaneModelClass(const PlaneModelClass&);
	~PlaneModelClass(void);
	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	//顶点缓冲和顶点索引缓冲
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

