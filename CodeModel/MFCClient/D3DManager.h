/*************************************************
// ����: ��Ⱥ��  2014/12/11
// ����: 3D���������
//*************************************************/
#pragma once

class CD3D11Class;
class CCamera;

class CD3DManager
{
protected:
	CD3DManager();
	virtual ~CD3DManager();
public:
	//�õ�
	static CD3DManager* Instance();

	//��ʼ������
	bool InitEnger(HWND hwnd);

	//��Ⱦ
	void Render();

	// �õ���ǰ���
	CCamera* getCamera();
	
	//�ر�
	void Shutdown();

protected:
	//����һ��D3DClass���Ա����
	CD3D11Class* m_p3dRoot;
};


