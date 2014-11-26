#include "stdafx.h"
#include "Camera.h"


CCamera::CCamera()
{
	_cameraType = AIRCRAFT;

	_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}


CCamera::~CCamera()
{
}

void CCamera::strafe(float units)
{
	// ����x,zƽ���ƶ�
	if (_cameraType == LANDOBJECT)
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;

	if (_cameraType == AIRCRAFT)
		_pos += _right * units;
}

void CCamera::fly(float units)
{
	// ����y���ƶ�
	if (_cameraType == LANDOBJECT)
		_pos.y += units;

	if (_cameraType == AIRCRAFT)
		_pos += _up * units;
}

void CCamera::walk(float units)
{
	// ����x,zƽ���ƶ�
	if (_cameraType == LANDOBJECT)
		_pos += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;

	if (_cameraType == AIRCRAFT)
		_pos += _look * units;
}

void CCamera::roll(float angle)
{
	//ֻ��aircraftģʽ����roll��ת
	if (_cameraType == AIRCRAFT)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &_look, angle);

		// ����look��������תup��right
		D3DXVec3TransformCoord(&_right, &_right, &T);
		D3DXVec3TransformCoord(&_up, &_up, &T);
	}
}

void CCamera::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_right, angle);

	// ����right��������תup��look
	D3DXVec3TransformCoord(&_up, &_up, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}

void CCamera::yaw(float angle)
{
	D3DXMATRIX T;

	//��LANDOBJECT,��������(0,1,0)��ת��
	if (_cameraType == LANDOBJECT)
		D3DXMatrixRotationY(&T, angle);

	//����aircraft,����up������ת
	if (_cameraType == AIRCRAFT)
		D3DXMatrixRotationAxis(&T, &_up, angle);

	// ����up����y�ᣬ��תright��look
	D3DXVec3TransformCoord(&_right, &_right, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}

void CCamera::getViewMatrix(D3DXMATRIX* V)
{
	// ����view�ֲ�����ϵ������ı˴�����
	D3DXVec3Normalize(&_look, &_look);
	// look X right
	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);

	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);

	// ����view����:
	float x = -D3DXVec3Dot(&_right, &_pos);
	float y = -D3DXVec3Dot(&_up, &_pos);
	float z = -D3DXVec3Dot(&_look, &_pos);

	(*V)(0, 0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
	(*V)(1, 0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
	(*V)(2, 0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
	(*V)(3, 0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
}

void CCamera::setCameraType(CameraType cameraType)
{
	_cameraType = cameraType;
}

void CCamera::getPosition(D3DXVECTOR3* pos)
{
	*pos = _pos;
}

void CCamera::setPosition(D3DXVECTOR3* pos)
{
	_pos = *pos;
}

void CCamera::getRight(D3DXVECTOR3* right)
{
	*right = _right;
}

void CCamera::getUp(D3DXVECTOR3* up)
{
	*up = _up;
}

void CCamera::getLook(D3DXVECTOR3* look)
{
	*look = _look;
}

void CCamera::reCorver()
{
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}