#include "baseSetup.h"


baseSetup::baseSetup(void)
{
	_userInput = 0;
	_graphics = 0;
}

baseSetup::baseSetup(const baseSetup& other)
{
}


baseSetup::~baseSetup(void)
{
}

bool baseSetup::Intialize()
{
	int width;
	int height;
	bool check;

	InitializeWindows(width,height);

	_userInput = new userInput;
	if(!_userInput)
	{
		MessageBox(_hwnd,NULL,L"UserInput failed to start",MB_OK);
		return false;
	}

	_userInput->Initialize();

	_graphics = new graphics;

	if(!_graphics)
	{
		MessageBox(_hwnd,NULL,L"Graphics failed to create",MB_OK);
		return false;
	}

	check = _graphics->Intialize(width,height,_hwnd);

	if(!check)
	{
		MessageBox(_hwnd,NULL,L"Graphics failed to intialize",MB_OK);
		return false;
	}

	return true;


}

void baseSetup::Shutdown()
{
	if(_graphics)
	{
		_graphics->Shutdown();
		delete _graphics;
		_graphics = 0;
	}

	if(_userInput)
	{
		delete _userInput;
		_userInput = 0;
	}

	ShutDownWindows();

	return;
}

void baseSetup::Run()
{
	MSG msg;
	bool done = false;
	bool result;

	ZeroMemory(&msg, sizeof(MSG));

	
	while(!done)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			
			result = Frame();
			if(!result)
			{
				done = true;
			}
		}


	}

	return;
}


bool baseSetup::Frame()
{
	bool check;

	if(_userInput->IskeyDown(VK_ESCAPE))
	{
		
		return false;
	}

	check = _graphics->Frame();

	if(!check)
	{
		MessageBox(_hwnd,NULL,L"graphics frame processing failed",MB_OK);
		return false;
	}

	return true;

}

LRESULT CALLBACK baseSetup::MessageHandler(HWND hwnd,UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		case WM_KEYDOWN:
		{
			//sends if key has been pressed to user input 
			_userInput->KeyDown((unsigned int)wparam);
			return 0;
		}

		case WM_KEYUP:
		{
			_userInput->KeyUp((unsigned int)wparam);
			return 0;
		}

		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}



	}

}


void baseSetup::InitializeWindows(int& screenWidth,int& screenHeight)
{
	// same as a Myregistery class in win32 app

	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX;
	int posY;


	ApllicationHandle = this;
	_hinstance = GetModuleHandle(NULL);

	_applicationName = L"Assignment2";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	// this sections sorts the screen size

	//pointless adding these in here?
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if(FULL_SCREEN)
	{
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = 0; 
		posY = 0;
	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;

		posX = 0;
		posY = 0;

	}

	_hwnd = CreateWindowEx(WS_EX_APPWINDOW, _applicationName, _applicationName, 
				WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
				posX, posY, screenWidth, screenHeight, NULL, NULL, _hinstance, NULL);

	ShowWindow(_hwnd, SW_SHOW);
	SetForegroundWindow(_hwnd);
	SetFocus(_hwnd);

	ShowCursor(false);

	return;
}


void baseSetup::ShutDownWindows()
{
	ShowCursor(true);

	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL,0);
	}

	DestroyWindow(_hwnd);
	UnregisterClass(_applicationName,_hinstance);
	_hinstance = NULL;

	ApllicationHandle = NULL;

	return;

}


LRESULT CALLBACK WndProc(HWND hwnd,UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
		{
			return ApllicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}

	}

}
