#pragma once
//����ú��ܹ�����windowsͷ�ļ��Ĵ�С��ʹ������������һЩ����Ҫ���ļ����ӿ����ʱ��
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "common.h"
#include "GraphicsClass.h"
#include "TimerClass.h"

const float PI = 3.14159265358979323f;

class SystemClass
	{
	public:
		SystemClass(void);
		SystemClass(const SystemClass &);
		~SystemClass(void);

		bool Initialize();
		void Shutdown();
		void Run();

		LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	private:
		bool Frame();
		void InitializeWindows(int&, int&);
		void ShutdownWindows();

		LPCWSTR m_applicationName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;

		//InputClass��GraphicsClass��SystemClass��������Ա����
		GraphicsClass* m_Graphics;
		//��ʱ��
		TimerClass *m_Timer;

		//����������ʱ,��¼��굱ǰ��λ
		POINT m_OldMousePos;

		//������״̬
		enum E_WheelStatus
		{
			eWheelNull = 0,		//��
			eWheelFront = 1,	//ǰ
			eWheelBack = 2		//��
		};
		E_WheelStatus m_wheelStatus;
	};

//���徲̬�Ļص������Լ�Ӧ�ó�����
//��Ϊ���崰����ʱ�򣬱���ָ�����ڻص����������������þ�̬�Ļص�����WndProc
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;