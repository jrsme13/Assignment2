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

	//_userInput->Intialize();

	_graphics = new graphics;

	if(!_graphics)
	{
		MessageBox(_hwnd,NULL,L"Graphics failed to create",MB_OK);
		return false;
	}

	//check = _graphics->Intialize(width,height,_hwnd);

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
		//_graphics->shutdown();
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

	if(_userInput->PressDownKey(VK_ESCAPE))
	{
		MessageBox(_hwnd,NULL,L"Escape failed",MB_OK);
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

