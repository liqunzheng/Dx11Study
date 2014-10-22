#pragma once
#include "common.h"

class ModelClass
	{
	private:
		struct VertexType
			{
			D3DXVECTOR3 position;
			D3DXVECTOR4 color;
			};

	public:
		ModelClass(void);
		ModelClass(const ModelClass &);

		~ModelClass(void);
		bool Initialize(ID3D11Device*, int, int, float);
		void Shutdown();
		void Render(ID3D11DeviceContext*);

		int GetIndexCount();
		//���ݶ����xֵ��zֵ�������yֵ
		float getHeight(float x, float z)const;
	private:
		bool InitializeBuffers(ID3D11Device*, int, int, float);
		void ShutdownBuffers();
		void RenderBuffers(ID3D11DeviceContext*);
		//���㻺��Ͷ�����������
		ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
		int m_vertexCount, m_indexCount;
	};

