#pragma once

//得到shader文件完整目录
std::wstring getShaderFile(const std::wstring& strName);

//得到texture文件完整目录
std::wstring getTextureFile(const std::wstring& strTexture);

//得到运行目录
std::wstring getExecFolder();

//创建索引缓存
ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pDevice, const CIndexArray& indices);

//创建顶点缓存
template<class pt>
ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pDevice, const std::vector<pt>& vertices)
{
	// 设置顶点缓冲描述
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(pt)* vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 指向保存顶点数据的临时缓冲.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 创建顶点缓冲.
	ID3D11Buffer *pBuffer = nullptr;
	HRESULT result = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &pBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}
	return pBuffer;
}

//D3D错误检测
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

//创建顶点着色器和输入布局
bool CreateVsInputLayout(const std::wstring& strVsName, const std::vector<std::string>& strsInputElement,ID3D11Device* pDevice,
	ID3D11VertexShader** vs, ID3D11InputLayout** input, const std::string& mainfun = "main", const std::string& version = "vs_5_0");

//创建像素着色器




