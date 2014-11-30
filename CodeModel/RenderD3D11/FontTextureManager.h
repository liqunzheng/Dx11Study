//*******************************************************************************
// ���ܣ�����Freetype2�������ɵ����� ʹ��Freetype2.5.3
// ---------------
// ���ߣ���Ⱥ��
//*******************************************************************************
#pragma once
#include "ft2build.h" 
#include "MtlShape2D.h"
#include FT_FREETYPE_H

class CD3D11Class;
class CTextureManager;

const int TEXTURE_SIZE = 1000; //һ��������ĳߴ�
const int FONT_SIZE = 50;//ÿһ��С����ĳߴ�
const int TEX_UINT_LEN = TEXTURE_SIZE / FONT_SIZE; //ÿһ���������С������

class CFontTextureManager
{
public:
	typedef struct _CHAR_TEX
	{
		unsigned long c;
		long row;  //texture row  
		long col;   //texture col  
		long width;//char width  
		long height;//char height  
		bool used;
	}CHAR_TEX, *LPCHAR_TEX;

	CFontTextureManager();
	~CFontTextureManager();

	bool Initialize(CD3D11Class*);
	void Shutdown();

	void CalTextData(const std::string& str, const int& size, const RECT& rect,
		const D3DXVECTOR4& clr,	std::vector<pt_xyz_c_uv>& vertices, CIndexArray& indices);

protected:
	void init(const std::string& fontFile);
	ID3D11Texture2D* getWCharTexture(wchar_t ch, int& width, int& height);
	bool copyCharToTexture(LPCHAR_TEX pCharTex);

	CTextureManager* m_ptexturmanager;
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pDeviceContext;
	ID3D11Texture2D *m_pTexture;
	ID3D11ShaderResourceView *m_shaderResourceView;

	FT_Library          m_FT2Lib;
	FT_Face             m_FT_Face;

	//������ã��Ƚ��ȳ�����̭ԭ��  
	long _texUnitIndex;//���һ����Ԫ����ʹ�õ�����

	LPCHAR_TEX* _pTexIndices;   //�����Ӧ�ַ�����  
	LPCHAR_TEX* _pCharIndices;  //�ַ���Ӧ��������
};

