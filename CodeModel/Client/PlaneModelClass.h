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
		D3DXVECTOR3 normal; //����
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
	//���㻺��Ͷ�����������
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

