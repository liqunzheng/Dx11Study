/*************************************************
// 作者: 李群正  2014/12/12
// 描述: 加载模型文件
//*************************************************/
#pragma once


class CShapeManager;
class IShape;

class CLoadShape
{
protected:
	CLoadShape();
	~CLoadShape();

public:
	static CLoadShape *Instance();
	std::vector<IShape*> load(CShapeManager* pShapeManager, const std::wstring& strFileName);
private:
	IShape* cratept_xyz_c(CShapeManager* pShapeManager, std::ifstream& fin);

};

