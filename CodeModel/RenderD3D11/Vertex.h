//*******************************************************************************
// ���ܣ���ӳ��ö����ʽ
// ---------------
// ���ߣ���Ⱥ��
// ʱ�䣺2014��11��30��
// xyz	��ά����
// nxyz ������
// xy	��ά����
// c	������ɫ
// uv	��������
//*******************************************************************************
#pragma once

struct  pt_xyz_c_uv
{
	D3DXVECTOR3 position;
	D3DXVECTOR4 color;
	D3DXVECTOR2 texture;
};

struct  pt_xyz_uv
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
};

struct pt_xyz_c
{
	D3DXVECTOR3 position;
	D3DXVECTOR4 color;
};

struct pt_xyz_nxyz_c
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR4 color;
};

struct pt_xyz_nxyz_uv
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texture;
};

struct pt_xyz_nxyz
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
};

typedef std::vector<unsigned int> CIndexArray;

// �ܱ任���� ���ڴ���shader
struct TranslateMatrix
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
};
