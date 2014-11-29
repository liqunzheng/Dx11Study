#include "stdafx.h"
#include "FontShape.h"
#include FT_GLYPH_H
#include "TextureManager.h"
#include "D3D11Class.h"
#include "TraceWin.h"


CFontShape::CFontShape()
{
	m_strFontFile = L"msyh.ttf";
	m_ftSize = 32;
	initFreetype2();
}


CFontShape::~CFontShape()
{
	shutdownFreetype2();
}

bool CFontShape::Initialize(CD3D11Class* p3DRoot)
{
	m_pDevice = p3DRoot->GetDevice();
	m_ptexturmanager = p3DRoot->getTextureMgr();
	return true;
}

void CFontShape::Shutdown()
{
}

bool CFontShape::BindRender(CD3D11Class*)
{
	throw std::logic_error("The method or operation is not implemented.");
}

D3DXMATRIX CFontShape::GetWorldMatrix() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::wstring CFontShape::GetMtlName() const
{
	return L"";
}

std::wstring CFontShape::GetShaderName() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

UINT CFontShape::GetIndexCount() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

void CFontShape::initFreetype2()
{
	//初始化FreeType库.. 
	if (FT_Init_FreeType(&m_ftLibrary))
		throw std::runtime_error("FT_Init_FreeType failed");

	//加载一个字体,取默认的Face,一般为Regualer 
	if (FT_New_Face(m_ftLibrary, "msyh.ttf", 0, &m_ftFace))
		throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");

	//大小要乘64.这是规定。照做就可以了。 
	FT_Set_Char_Size(m_ftFace, m_ftSize << 6, m_ftSize << 6, 96, 96);
}

void CFontShape::shutdownFreetype2()
{
	FT_Done_Face(m_ftFace);
	FT_Done_FreeType(m_ftLibrary);
}

bool CFontShape::DrawAUnicode(wchar_t ch)
{
	if (FT_Load_Glyph(m_ftFace, FT_Get_Char_Index(m_ftFace, ch), FT_LOAD_DEFAULT))
		throw std::runtime_error("FT_Load_Glyph failed");

	//得到字模 
	FT_Glyph glyph;
	if (FT_Get_Glyph(m_ftFace->glyph, &glyph))
		throw std::runtime_error("FT_Get_Glyph failed");

	//转化成位图 
	FT_Render_Glyph(m_ftFace->glyph, FT_RENDER_MODE_NORMAL);
	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//取道位图数据 
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;

	//把位图数据拷贝自己定义的数据区里.这样旧可以画到需要的东西上面了。 
	int width = bitmap.width;
	int height = bitmap.rows;

	unsigned char *pBuf = new unsigned char[width * height * 4];
	ZeroMemory(pBuf, width * height * 4);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			char _vl = bitmap.buffer[i + width*j];
			if (_vl != '\0')
			{
				pBuf[4 * (i + width*j) + 3] = _vl;
			}
		}
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = width;
	textureDesc.Height = height;
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
	a.SysMemPitch = 4 * width;
	a.SysMemSlicePitch = 0;

	// Create the render target texture.
	HRESULT result = m_pDevice->CreateTexture2D(&textureDesc, &a, &m_pTexture);
	delete[] pBuf;

	if (FAILED(result))
	{
		return false;
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
		return false;
	}
	m_ptexturmanager->add(L"Font", m_shaderResourceView);
	return true;
}
