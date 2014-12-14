#pragma once

//�õ�shader�ļ�����Ŀ¼
std::wstring getShaderFile(const std::wstring& strName);

//�õ�texture�ļ�����Ŀ¼
std::wstring getTextureFile(const std::wstring& strTexture);

//�õ�����Ŀ¼
std::wstring getExecFolder();

//������������
ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pDevice, const CIndexArray& indices);

//�������㻺��
template<class pt>
ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pDevice, const std::vector<pt>& vertices)
{
	// ���ö��㻺������
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(pt)* vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// ָ�򱣴涥�����ݵ���ʱ����.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// �������㻺��.
	ID3D11Buffer *pBuffer = nullptr;
	HRESULT result = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &pBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}
	return pBuffer;
}

//D3D������
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
{                                                          \
	HRESULT hr = (x);                                      \
if (FAILED(hr))                                         \
{                                                      \
	DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
}                                                      \
}
#endif
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) if (ptr != nullptr) { delete ptr; ptr = nullptr;}
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(ptr) if (ptr != nullptr) { ptr->Release(); ptr = nullptr;}
#endif

//����������ɫ�������벼��
bool CreateVsInputLayout(const std::wstring& strVsName, const std::vector<std::string>& strsInputElement,ID3D11Device* pDevice,
	ID3D11VertexShader** vs, ID3D11InputLayout** input, const std::string& mainfun = "main", const std::string& version = "vs_5_0");

//����������ɫ��




