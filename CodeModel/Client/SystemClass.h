#pragma once
//定义该宏能够减少windows头文件的大小，使编译器不编译一些不必要的文件，加快编译时间
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

		//InputClass和GraphicsClass是SystemClass的两个成员变量
		GraphicsClass* m_Graphics;
		//计时器
		TimerClass *m_Timer;

		//按下鼠标左键时,记录鼠标当前的位
		POINT m_OldMousePos;

		//鼠标滚轮状态
		enum E_WheelStatus
		{
			eWheelNull = 0,		//空
			eWheelFront = 1,	//前
			eWheelBack = 2		//后
		};
		E_WheelStatus m_wheelStatus;
	};

//定义静态的回调函数以及应用程序句柄
//因为定义窗口类时候，必须指定窗口回调函数，所以我们用静态的回调函数WndProc
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;