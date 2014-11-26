#pragma once
#include "RenderExport.h"

class RENDERD3D11_API CCamera
{
public:
	//֧�����������ģ�� AIRCRAFT �����ڿռ������˶�,����6�����ɶ�
	// LANDOBJECT ��ĳЩ�ض�������ƶ�
	enum CameraType { LANDOBJECT, AIRCRAFT };
	CCamera();
	~CCamera();

	void strafe(float units); // ����
	void fly(float units);    // ����
	void walk(float units);   // ǰ��

	void pitch(float angle); // ��תview����ϵright����
	void yaw(float angle);   // ��תup����
	void roll(float angle);  // ��תlook����

	void getViewMatrix(D3DXMATRIX* V);
	void setCameraType(CameraType cameraType);
	void getPosition(D3DXVECTOR3* pos);
	void setPosition(D3DXVECTOR3* pos);

	void getRight(D3DXVECTOR3* right);
	void getUp(D3DXVECTOR3* up);
	void getLook(D3DXVECTOR3* look);

	void reCorver();
private:
	CameraType  _cameraType;
	D3DXVECTOR3 _right;
	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look;
	D3DXVECTOR3 _pos;
};

