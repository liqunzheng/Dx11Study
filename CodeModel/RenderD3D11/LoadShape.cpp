#include "stdafx.h"
#include "LoadShape.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include "ColorShape.h"
#include "StringHelp.h"

CLoadShape::CLoadShape()
{
}


CLoadShape::~CLoadShape()
{
}

CLoadShape * CLoadShape::Instance()
{
	static CLoadShape myself;
	return &myself;
}

std::vector<IShape*> CLoadShape::load(CShapeManager* pShapeManager, const std::wstring& strFileName)
{
	std::vector<IShape*> shapesRet;

	std::ifstream fin(strFileName, std::ios::in);
	if (!fin)
	{
		throw std::logic_error("文件不存在!");
	}

	char line[1024] = { 0 };
	
	//读文件头
	if (!fin.getline(line, sizeof(line)))
	{
		throw std::logic_error("文件打开失败");
	}
	std::string str = line;
	if (str != "LEE_PT_FORMAT")
	{
		throw std::logic_error("无法识别的文件格式");
	}

	//读顶点格式
	if (!fin.getline(line, sizeof(line)))
	{
		throw std::logic_error("文件读取失败");
	}
	str = line;

	while (str != "LEE_PT_END")
	{
		if (str == "pt_xyz_c_uv")
		{
		}
		if (str == "pt_xyz_uv")
		{
		}
		if (str == "pt_xyz_c")
		{
			IShape* pShape = cratept_xyz_c(pShapeManager, fin);
			shapesRet.push_back(pShape);
		}
		if (str == "pt_xyz_nxyz_c")
		{
		}
		if (str == "pt_xyz_nxyz_uv")
		{
		}

		if (!fin.getline(line, sizeof(line)))
		{
			throw std::logic_error("文件读取失败");
		}
		str = line;
	}

	fin.clear();
	fin.close();

	return shapesRet;
}

IShape* CLoadShape::cratept_xyz_c(CShapeManager* pShapeManager, std::ifstream& fin)
{
	CColorShape *pShape = new CColorShape();
	char line[1024] = { 0 };

	if (!fin.getline(line, sizeof(line)))
	{
		throw std::logic_error("文件读取失败");
	}
	std::string str = line;
	std::vector<std::string> strs = split(str, " ");
	assert(strs.size() == 2);

	int nVertices = str2int(strs[0]);
	int nTriangles = str2int(strs[1]);

	pShape->m_vertices.resize(nVertices);
	pShape->m_indices.resize(3 * nTriangles);

	for (int i = 0; i < nVertices; ++i)
	{
		fin.getline(line, sizeof(line));
		str = line;
		strs = split(str, " ");
		float x = str2float(strs[0]);
		float y = str2float(strs[1]);
		float z = str2float(strs[2]);
		float r = str2float(strs[3]) / 255.0f;
		float g = str2float(strs[4]) / 255.0f;
		float b = str2float(strs[5]) / 255.0f;
		float a = str2float(strs[6]);

		pShape->m_vertices[i].position = { x, y, z };
		pShape->m_vertices[i].color = { r, g, b, a };
	}

	for (int i = 0; i < nTriangles; ++i)
	{
		fin.getline(line, sizeof(line));
		str = line;
		strs = split(str, " ");
		int n0 = str2int(strs[0]);
		int n1 = str2int(strs[1]);
		int n2 = str2int(strs[2]);
		pShape->m_indices[3 * i] = n0;
		pShape->m_indices[3 * i + 1] = n1;
		pShape->m_indices[3 * i + 2] = n2;
	}

	return pShape;
}
