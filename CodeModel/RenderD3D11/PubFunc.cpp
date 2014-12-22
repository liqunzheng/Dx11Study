#include "stdafx.h"
#include "PubFunc.h"

std::wstring getShaderFile(const std::wstring& strName)
{
	std::wstring strPath;
	//WCHAR ch[_MAX_PATH];
	//_wgetcwd(ch, _MAX_PATH);
	//strPath = ch;

	//strPath = L"";
	return strPath;
}

std::wstring getTextureFile(const std::wstring& strTexture)
{
	std::wstring strPath;
	//WCHAR ch[_MAX_PATH];
	//_wgetcwd(ch, _MAX_PATH);
	//strPath = ch;

	//strPath = L"";
	return strPath;
}

std::wstring getCurrentFolder()
{
	std::wstring strPath;
	WCHAR ch[_MAX_PATH];
	_wgetcwd(ch, _MAX_PATH);
	strPath = ch;
	return strPath;
}

ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pDevice, const CIndexArray& indices)
{
	// 设置索引缓冲描述.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int)* indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 指向存临时索引缓冲.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 创建索引缓冲.
	ID3D11Buffer* pBuffer = nullptr;
	HRESULT result = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &pBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}

	return pBuffer;
}

bool CreateVsInputLayout(const std::wstring& strVsName, const std::vector<std::string>& strsInputElement, ID3D11Device* pDevice,
	ID3D11VertexShader** vs, ID3D11InputLayout** input, const std::string& mainfun /*= "main"*/, const std::string& version /*= "vs_5_0"*/)
{
	ID3D10Blob* errorMessage = nullptr;
	std::wstring strErrorMessage;

	// 编译vs代码.
	ID3D10Blob* vertexShaderBuffer = 0;
	HRESULT result = D3DX11CompileFromFile(strVsName.c_str(), NULL, NULL, mainfun.c_str(), version.c_str(),
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	HR(result);

	// 从缓冲创建vs shader.
	result = pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, vs);
	HR(result);

	// 设置数据布局，以便在shader中使用.
	// 定义要和ModelClass中的顶点结构一致.
	D3D11_INPUT_ELEMENT_DESC *InputLayout = new D3D11_INPUT_ELEMENT_DESC[strsInputElement.size()];
	for (size_t i = 0; i < strsInputElement.size(); ++i)
	{
		std::string strElement = strsInputElement[i];
		if (strElement == "POSITION")
		{
			InputLayout[i] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}
		else if (strElement == "COLOR")
		{
			InputLayout[i] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}
		else if (strElement == "NORMAL")
		{
			InputLayout[i] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}
		else
		{
			assert(false);
		}
	}

	// 创建顶点输入布局.
	result = pDevice->CreateInputLayout(InputLayout, strsInputElement.size(), vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), input);
	if (FAILED(result))
	{
		return false;
	}

	//释放顶点和像素缓冲.
	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(errorMessage);

	return true;
}

bool CreatePs(const std::wstring& strPsName, ID3D11Device* pDevice, ID3D11VertexShader** ps, const std::string& mainfun /*= "main"*/, const std::string& version /*= "vs_5_0"*/)
{
	ID3D10Blob* errorMessage = nullptr;
	std::wstring strErrorMessage;

	// 编译ps代码.
	ID3D10Blob* pixelShaderBuffer = nullptr;
	HRESULT result = D3DX11CompileFromFile(strPsName.c_str(), NULL, NULL, mainfun.c_str(), version.c_str(),
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	HR(result);

	// 从缓冲创建vs shader.
	result = pDevice->CreateVertexShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, ps);
	HR(result);

	SAFE_RELEASE(pixelShaderBuffer);
	SAFE_RELEASE(errorMessage);
	return true;
}

