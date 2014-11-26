#include "SystemClass.h"
#include "Camera.h"
#include "D3D11Class.h"


SystemClass::SystemClass(void)
{
	m_Input = 0;
	m_Graphics = 0;
	m_Timer = 0;
}

SystemClass::SystemClass(const SystemClass &)
{

}

SystemClass::~SystemClass(void)
{
}

//���ô��ڳ�ʼ������������һЩ��ĳ�ʼ������
bool SystemClass::Initialize()
{
	bool result;
	int screenWidth = 0, screenHeight = 0;

	// ��ʼ������
	InitializeWindows(screenWidth, screenHeight);

	//����input�������������
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// ��ʼ���������.
	m_Input->Initialize();

	// ����ͼ�ζ������������ȾӦ�ó����е���������
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}
	// ��ʼ��ͼ�ζ���
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	//������ʱ������
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	//��ʼ����ʱ������
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	//�������һЩ���ٹ���
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}
	// ִ��һЩ���ٹ���.
	ShutdownWindows();
}

//������Ϣ
void  SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// ��ʼ����Ϣ�ṹ.
	ZeroMemory(&msg, sizeof(MSG));

	// ѭ��������Ϣ����������յ�WM_QUIT.
	done = false;
	while (!done)
	{
		// ����windows��Ϣ.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// ���յ�WM_QUIT��Ϣ���˳�����.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//���ǵ���Ⱦ���������������Է������
			//....
			//.....
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
	return;
}

bool SystemClass::Frame()
{
	bool result;

	//����û��Ƿ���ESC����������£��˳�����.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//���A,S,D,W,Q,E,Z,X,C�����£��ƶ������ 
	if (GetAsyncKeyState('W') & 0x8000)    //ǰ�� 
		m_Graphics->getCamera()->walk(-0.1f);
	if (GetAsyncKeyState('S') & 0x8000)
		m_Graphics->getCamera()->walk(0.1f);
	if (GetAsyncKeyState('A') & 0x8000)    //���� 
		m_Graphics->getCamera()->strafe(-0.1f);
	if (GetAsyncKeyState('D') & 0x8000)
		m_Graphics->getCamera()->strafe(0.1f);
	if (GetAsyncKeyState('Q') & 0x8000)    //���� 
		m_Graphics->getCamera()->fly(-0.1f);
	if (GetAsyncKeyState('E') & 0x8000)
		m_Graphics->getCamera()->fly(0.1f);
	if (GetAsyncKeyState('Z') & 0x8000)
		m_Graphics->getCamera()->pitch(PI / 180);
	if (GetAsyncKeyState('X') & 0x8000)
		m_Graphics->getCamera()->yaw(PI / 180);
	if (GetAsyncKeyState('C') & 0x8000)
		m_Graphics->getCamera()->roll(PI / 180);
	if (GetAsyncKeyState('R') & 0x8000)
		m_Graphics->getCamera()->reCorver();

	m_Timer->Frame();

	////��������ת�����
	//m_Graphics->m_Camera->roll(m_Timer->GetTime() / 1000);

	// ִ��֡��Ⱦ����.
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}
	return true;
}

//��ʼ�������࣬����Ӧ�ó��򴰿�
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//�õ�Ӧ�ó������ָ��
	ApplicationHandle = this;

	// �õ�Ӧ�ó���ʵ�����
	m_hinstance = GetModuleHandle(NULL);

	// Ӧ�ó�������
	m_applicationName = L"Engine";

	// ���ô��������.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc; //ָ���ص�����
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //Ĭ�Ϻ�ɫ���ں�ɫ����
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// ע�ᴰ����
	RegisterClassEx(&wc);

	// �õ�windows����ֱ���
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// �����Ƿ�ȫ�����ò�ͬ�ķֱ���.
	if (FULL_SCREEN)
	{
		//ȫ��ģʽ�£����ô��ڴ�СΪwindows����ֱ���.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// ��ʱ������ʾ�豸Ϊȫ��ģʽ��ע�⣺Ӧ�ó����˳�ʱ�򣬽��ָ�ϵͳĬ�����á�
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// ���ô��ڵ�Ĭ��λ��Ϊ(0,0).
		posX = posY = 0;
	}
	else
	{
		// ����ģʽ��800*600.
		screenWidth = 800;
		screenHeight = 600;

		// ����λ��,posX, posY�������Ͻ�����
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// ȫ���ʹ���ʹ�ò�ͬ�Ĳ���.
	if (FULL_SCREEN)
	{
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	}
	else
	{
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_OVERLAPPEDWINDOW,
			posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	}

	// ��ʾ���ڲ�������Ϊ����.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// �������.
	//ShowCursor(false);
}

void SystemClass::ShutdownWindows()
{
	//��ʾ���.
	//ShowCursor(true);

	// �ָ�Ĭ����ʾ����.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// ���ٴ���
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ����Ӧ�ó���ʵ��.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;
	ApplicationHandle = NULL;

	return;

}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		//�����Ϣ
	case WM_LBUTTONDOWN:
		if (wparam & MK_LBUTTON)
		{
			SetCapture(hwnd);
			m_OldMousePos.x = LOWORD(lparam);
			m_OldMousePos.y = HIWORD(lparam);

			m_Graphics->pick(m_OldMousePos.x, m_OldMousePos.y);
		}
		return 0;
	case WM_LBUTTONUP:
		ReleaseCapture();
		return 0;
	case WM_MOUSEMOVE:
		if (wparam & MK_LBUTTON)
		{
			POINT mousePos;
			mousePos.x = (int)LOWORD(lparam);
			mousePos.y = (int)HIWORD(lparam);

			int dx = mousePos.x - m_OldMousePos.x;
			int dy = mousePos.y - m_OldMousePos.y;
			
			////ͨ�����ת�������
			//m_Graphics->m_Camera->pitch(dy * 0.0087266f);
			//m_Graphics->m_Camera->yaw(dx * 0.0087266f);

			m_OldMousePos = mousePos;
		}
		return 0;

	// ��ⰴ����Ϣ.
	case WM_KEYDOWN:
		m_Input->KeyDown((unsigned int)wparam);
		return 0;

	case WM_KEYUP:
		m_Input->KeyUp((unsigned int)wparam);
		return 0;

	//�κ�������Ϣ���͵�windowsȱʡ����.
	case WM_SIZE:
	{
					int screenWidth = 0, screenHeight = 0;
					screenWidth = LOWORD(lparam);
					screenHeight = HIWORD(lparam);
					if (screenHeight == 0)
					{
						screenHeight = 1;
					}
					if (screenWidth == 0)
					{
						screenWidth = 1;
					}
					// ���ڴ�С�ı�ʱ�����³�ʼ��ͼ�ζ���
					if (m_Graphics)
					{
						bool result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
						if (!result)
						{
							return false;
						}
					}
					return 0;
	}
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// ����������Ϣ.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// ���ڹر���Ϣ.
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		//MessageHandle���̴�������������Ϣ.
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}