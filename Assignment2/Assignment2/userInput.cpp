#include "userInput.h"


userInput::userInput()
{
}

userInput::userInput(const userInput& other)
{
}

userInput::~userInput()
{
}

void userInput::Initialize()
{
	for(int i = 0; i<256 ;i++)
	{
		_keys[i]= false;
	}

	return;
}

void userInput::KeyDown(unsigned int input)
{

	_keys[input] = true;
	return;
}

void userInput::KeyUp(unsigned int input)
{
	_keys[input] = false;
	return;
}

bool userInput::IskeyDown(unsigned int key)
{

	return _keys[key];
}

