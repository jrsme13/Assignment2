#pragma once

#ifndef _USERINPUT_H_
#define _USERINPUT_H_


class userInput
{
public:
	userInput();
	userInput(const userInput&);
	~userInput();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IskeyDown(unsigned int);

private:

	bool _keys[256];
};

#endif