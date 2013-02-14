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
