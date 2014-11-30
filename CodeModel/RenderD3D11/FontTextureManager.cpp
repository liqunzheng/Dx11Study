#include "stdafx.h"
#include "FontTextureManager.h"
#include FT_GLYPH_H
#include "D3D11Class.h"
#include "TextureManager.h"


CFontTextureManager::CFontTextureManager()
: _texUnitIndex(-1)
, m_FT2Lib(NULL)
, m_FT_Face(NULL)
{
}

CFontTextureManager::~CFontTextureManager()
{
}

bool CFontTextureManager::Initialize(CD3D11Class* p3DRoot)
{
	m_pDevice = p3DRoot->GetDevice();
	m_pDeviceContext = p3DRoot->GetDeviceContext();
	m_ptexturmanager = p3DRoot->getTextureMgr();
	init("msyh.ttf");

	_pTexIndices = new LPCHAR_TEX[TEX_UINT_LEN * TEX_UINT_LEN];
	memset(_pTexIndices, 0, sizeof(_pTexIndices)* TEX_UINT_LEN * TEX_UINT_LEN);
	_pCharIndices = new LPCHAR_TEX[256 * 256];
	memset(_pCharIndices, 0, sizeof(_pCharIndices)* 256 * 256);

	return true;
}

void CFontTextureManager::CalTextData(const std::string& str, const int& size, const RECT& rect,
	const D3DXVECTOR4& clr, std::vector<pt_xyz_c_uv>& vertices, CIndexArray& indices)
{
	if (str.empty())
	{
		return;
	}

	float scale = (float)size / FONT_SIZE;
	int lineHeight = FONT_SIZE + 4;

	unsigned long charcode = 0;
	const char* p = str.c_str();
	long xPos = rect.left, yPos = rect.top;
	while (*p)
	{
		if (*p == '\n')
		{
			p++;
			xPos = rect.left;
			yPos += lineHeight * scale;
			continue;
		}
		if (*p & 0x80)
		{
			charcode = *(WORD*)p;
		}
		else
		{
			charcode = *(char*)p;
		}
		LPCHAR_TEX pCharTex = _pCharIndices[charcode];
		if (pCharTex == NULL)
		{
			_texUnitIndex++;
			if (_pTexIndices[_texUnitIndex] == NULL)
			{
				_pCharIndices[charcode] = new CHAR_TEX();
				_pCharIndices[charcode]->c = charcode;
				_pTexIndices[_texUnitIndex] = _pCharIndices[charcode];
			}
			else
			{
				//exchange texture  
				long tempIndex = _texUnitIndex;
				while (_pTexIndices[_texUnitIndex]->used)
				{
					if (_texUnitIndex == TEX_UINT_LEN * TEX_UINT_LEN)
					{
						_texUnitIndex = -1;
					}
					_texUnitIndex++;
					if (_texUnitIndex == tempIndex)
					{
						//all used  
						assert(false);
					}
				}
				_pTexIndices[_pTexIndices[_texUnitIndex]->c] = NULL;
				_pTexIndices[charcode] = _pTexIndices[_texUnitIndex];
			}
			_pCharIndices[charcode]->c = charcode;
			_pCharIndices[charcode]->used = true;
			_pCharIndices[charcode]->row = _texUnitIndex / TEX_UINT_LEN;
			_pCharIndices[charcode]->col = _texUnitIndex % TEX_UINT_LEN;
			copyCharToTexture(_pCharIndices[charcode]);
		}

		float x = xPos - 0.5;
		float y = yPos + (lineHeight - FONT_SIZE) *0.5 - 0.5;
		float addX = _pCharIndices[charcode]->width * scale;
		float addY = _pCharIndices[charcode]->height * scale;
		if (*p & 0x80) //双字节
		{
			p += 2;
		}
		else //单字节
		{
			p += 1;
		}
		xPos += _pCharIndices[charcode]->width * scale;

		//render
		int nIndexStart = vertices.size();
		std::vector<pt_xyz_c_uv> vecs;
		vecs.resize(4);
		float bround = 0.0f;
		float u = (float)(_pCharIndices[charcode]->col * FONT_SIZE) / TEXTURE_SIZE;
		float v = (float)(_pCharIndices[charcode]->row * FONT_SIZE) / TEXTURE_SIZE;
		float addU = (float)(_pCharIndices[charcode]->width) / TEXTURE_SIZE - bround;
		float addV = (float)(_pCharIndices[charcode]->height) / TEXTURE_SIZE - bround;
		vecs[0] = { D3DXVECTOR3(x, y, 0.0f),clr, D3DXVECTOR2(u, v+addV) };
		vecs[1] = { D3DXVECTOR3(x, y + addY, 0.0f),clr, D3DXVECTOR2(u, v) };
		vecs[2] = { D3DXVECTOR3(x + addX, y, 0.0f),clr, D3DXVECTOR2(u + addU, v+addV) };
		vecs[3] = { D3DXVECTOR3(x + addX, y + addY, 0.0f),clr, D3DXVECTOR2(u + addU, v) };
		vertices.insert(vertices.end(), vecs.begin(), vecs.end());
		indices.push_back(nIndexStart+0);
		indices.push_back(nIndexStart+1);
		indices.push_back(nIndexStart+2);
		indices.push_back(nIndexStart+2);
		indices.push_back(nIndexStart+1);
		indices.push_back(nIndexStart+3);
	}
}

void CFontTextureManager::init(const std::string& fontFile)
{
	if (FT_Init_FreeType(&m_FT2Lib))
	{
		FT_Done_FreeType(m_FT2Lib);
		m_FT2Lib = NULL;
		return;
	}

	if (FT_New_Face(m_FT2Lib, fontFile.c_str(), 0, &m_FT_Face))
	{
		FT_Done_FreeType(m_FT2Lib);
		m_FT2Lib = NULL;
		return;
	}

	FT_Select_Charmap(m_FT_Face, FT_ENCODING_UNICODE);
	FT_Set_Pixel_Sizes(m_FT_Face, FONT_SIZE, FONT_SIZE);

	BYTE *pBuf = new BYTE[TEXTURE_SIZE*TEXTURE_SIZE * 4];
	ZeroMemory(pBuf, TEXTURE_SIZE*TEXTURE_SIZE * 4);

	// Setup the render target texture description.
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = TEXTURE_SIZE;
	textureDesc.Height = TEXTURE_SIZE;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA a;
	a.pSysMem = pBuf;
	a.SysMemPitch = 4 * TEXTURE_SIZE;
	a.SysMemSlicePitch = 0;

	// Create the render target texture.
	HRESULT result = m_pDevice->CreateTexture2D(&textureDesc, &a, &m_pTexture);
	delete[] pBuf;

	if (FAILED(result))
	{
		throw std::logic_error("创建纹理失败");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	// Create the shader resource view.
	result = m_pDevice->CreateShaderResourceView(m_pTexture, &shaderResourceViewDesc, &m_shaderResourceView);
	if (FAILED(result))
	{
		throw std::logic_error("创建纹理视图失败");
	}

	m_ptexturmanager->add(L"uFont", m_shaderResourceView);
}

bool CFontTextureManager::copyCharToTexture(LPCHAR_TEX pCharTex)
{
	char*  chr = (char*)&(pCharTex->c);
	wchar_t wchar;

	MultiByteToWideChar(CP_ACP, 0, chr, strlen(chr) + 1, &wchar, 1);

	int width, height;
	ID3D11Texture2D* pText = getWCharTexture(wchar, width, height);
	assert(pText != nullptr);

	int row = pCharTex->row;
	int col = pCharTex->col;
	pCharTex->width = width;
	pCharTex->height = height;

	D3D11_BOX sourceRegion;
	sourceRegion.left = 0;
	sourceRegion.right = width;
	sourceRegion.top = 0;
	sourceRegion.bottom = height;
	sourceRegion.front = 0;
	sourceRegion.back = 1;

	int nDecX = col*FONT_SIZE;
	int nDecY = row*FONT_SIZE;

	m_pDeviceContext->CopySubresourceRegion(m_pTexture, 0, nDecX, nDecY, 0, pText, 0, &sourceRegion);
	pText->Release();
	pText = nullptr;
	return true;
}

ID3D11Texture2D* CFontTextureManager::getWCharTexture(wchar_t ch, int& owidth, int& oheight)
{
	if (FT_Load_Glyph(m_FT_Face, FT_Get_Char_Index(m_FT_Face, ch), FT_LOAD_DEFAULT))
		throw std::runtime_error("FT_Load_Glyph failed");

	//得到字模 
	FT_Glyph glyph;
	if (FT_Get_Glyph(m_FT_Face->glyph, &glyph))
		throw std::runtime_error("FT_Get_Glyph failed");

	//转化成位图 
	FT_Render_Glyph(m_FT_Face->glyph, FT_RENDER_MODE_NORMAL);
	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph; 
	FT_GlyphSlot slot = m_FT_Face->glyph;

	//取位图数据 
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;

	//把位图数据拷贝自己定义的数据区里.这样旧可以画到需要的东西上面了。 
	int width = bitmap.width;
	int height = bitmap.rows;

	owidth = m_FT_Face->glyph->metrics.horiAdvance >> 6;
	oheight= m_FT_Face->glyph->metrics.vertAdvance >> 6;

	int ntop = m_FT_Face->glyph->metrics.vertBearingY >> 6;
	int nleft = slot->bitmap_left;

	unsigned char *pBuf = new unsigned char[owidth * oheight * 4];
	ZeroMemory(pBuf, owidth * oheight * 4);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			char _vl = bitmap.buffer[i + width*j];
			if (_vl != '\0')
			{
				pBuf[4 * (nleft+i + owidth*(j+ntop)) + 3] = _vl;
			}
		}
	}

	// Setup the render target texture description.
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = owidth;
	textureDesc.Height = oheight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA a;
	a.pSysMem = pBuf;
	a.SysMemPitch = 4 * owidth;
	a.SysMemSlicePitch = 0;

	// Create the render target texture.
	ID3D11Texture2D * pCharText;
	HRESULT result = m_pDevice->CreateTexture2D(&textureDesc, &a, &pCharText);
	delete[] pBuf;
	if (FAILED(result))
	{
		throw std::runtime_error("创建字的纹理出错");
	}
	return pCharText;
}

void CFontTextureManager::Shutdown()
{
	if (_pTexIndices != nullptr)
	{
		delete[] _pTexIndices;
		_pTexIndices = nullptr;
	}

	if (_pCharIndices != nullptr)
	{
		delete[] _pCharIndices;
		_pCharIndices = nullptr;
	}
}

