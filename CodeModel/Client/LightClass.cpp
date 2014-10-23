#include "LightClass.h"


LightClass::LightClass(void)
{
	m_position = D3DXVECTOR3(1.0, 8.0, 1.0);
	m_globalAmbient = D3DXVECTOR3(1.0, 1.0, 1.0);
	m_direction = D3DXVECTOR3(-1.0, -1.0, 1.0);
	m_lightColor = D3DXVECTOR3(1.0, 1.0, 1.0);
	m_shininess = 5.0f;
}

LightClass::LightClass(const LightClass & others)
{

}

LightClass::~LightClass(void)
{
}

void  LightClass::SetLightPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);
}

//����ȫ�ֻ�����ϵ��
void LightClass::SetGlobalAmbient(float red, float green, float blue)
{
	m_globalAmbient = D3DXVECTOR3(red, green, blue);
	return;
}

//����ƽ�йⷽ��
void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
	return;
}

//���ù�Դ��ɫ
void LightClass::SetLightColor(float red, float green, float blue)
{
	m_lightColor = D3DXVECTOR3(red, green, blue);
	return;
}

//����ȫ�ֻ�����ϵ��
void LightClass::SetShininess(float shininess)
{
	m_shininess = shininess;
	return;
}

//�õ���Դλ��
D3DXVECTOR3 LightClass::GetPosition()
{
	return m_position;
}

//�õ�ȫ�ֻ�����ϵ��
D3DXVECTOR3 LightClass::GetGlobalAmbient()
{
	return m_globalAmbient;
}

//�õ�ƽ�йⷽ��
D3DXVECTOR3 LightClass::GetDirection()
{
	return m_direction;
}

//�õ���Դ��ɫ
D3DXVECTOR3 LightClass::GetLightColor()
{
	return m_lightColor;
}

//�õ�ȫ�ֻ�����ϵ��
float LightClass::GetShininess()
{
	return m_shininess;
}
