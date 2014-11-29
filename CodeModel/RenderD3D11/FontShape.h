#pragma once
#include "ft2build.h" 
#include FT_FREETYPE_H
#include "IShape.h"

class CFontShape : public IShape
{
public:
	CFontShape();
	~CFontShape();

	virtual bool Initialize(CD3D11Class*) override;

	virtual void Shutdown() override;

	virtual bool BindRender(CD3D11Class*) override;

	virtual D3DXMATRIX GetWorldMatrix() const override;

	virtual std::wstring GetMtlName() const override;

	virtual std::wstring GetShaderName() const override;

	virtual UINT GetIndexCount() const override;

	bool DrawAUnicode(wchar_t ch);

protected:
	void initFreetype2();
	void shutdownFreetype2();

	class CTextureManager* m_ptexturmanager;

	ID3D11Device *m_pDevice;
	ID3D11Texture2D *m_pTexture;
	ID3D11ShaderResourceView *m_shaderResourceView;

	std::wstring m_strFontFile;
	FT_Library m_ftLibrary;
	int m_ftSize;
	FT_Face m_ftFace;
};

