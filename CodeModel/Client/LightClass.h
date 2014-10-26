#pragma once
#include "common.h"

class LightClass
{
public:
	LightClass(void);
	LightClass(const LightClass &);
	~LightClass(void);

	void SetLightPosition(float, float, float, float);
	void SetGlobalAmbient(float, float, float, float);
	void SetDirection(float, float, float);
	void SetLightColor(float, float, float, float);
	void SetShininess(float);

	D3DXVECTOR4 GetPosition();
	D3DXVECTOR4 GetGlobalAmbient();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetLightColor();
	float GetShininess();

private:
	D3DXVECTOR4 m_position; //光源位置
	D3DXVECTOR4 m_globalAmbient; //全局环境光系数
	D3DXVECTOR3 m_direction; //光源方向,平行光
	D3DXVECTOR4 m_lightColor; //光源颜色
	float m_shininess;  //高光系数
};
