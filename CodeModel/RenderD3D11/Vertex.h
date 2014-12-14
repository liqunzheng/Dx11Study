//*******************************************************************************
// 功能：添加常用顶点格式
// ---------------
// 作者：李群政
// 时间：2014年11月30日
// xyz	三维坐标
// nxyz 法向量
// xy	二维坐标
// c	顶点颜色
// uv	纹理坐标
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

// 总变换矩阵 用于传给shader
struct TranslateMatrix
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
};
