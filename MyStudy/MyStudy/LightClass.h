#pragma once
#include "common.h"

class LightClass
{
public:
	LightClass(void);
	LightClass(const LightClass &);
	~LightClass(void);

	void SetLightPosition(float, float, float);
	void SetGlobalAmbient(float, float, float);
	void SetDirection(float, float, float);
	void SetLightColor(float, float, float);
	void SetShininess(float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetGlobalAmbient();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR3 GetLightColor();
	float GetShininess();

private:
	D3DXVECTOR3 m_position; //��Դλ��
	D3DXVECTOR3 m_globalAmbient; //ȫ�ֻ�����ϵ��
	D3DXVECTOR3 m_direction; //��Դ����,ƽ�й�
	D3DXVECTOR3 m_lightColor; //��Դ��ɫ
	float m_shininess;  //�߹�ϵ��
};

