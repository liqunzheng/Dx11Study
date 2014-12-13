/*************************************************
// ����: ��Ⱥ��  2014/12/12
// ����: ����ģ���ļ�
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

