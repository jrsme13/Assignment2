#pragma once

#ifndef _BASESETUP_H_
#define _BASESETUP_H_

// a faster WIN32 redueces less used API features
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "graphics.h"
#include "userInput.h"


class baseSetup
{
public:
	baseSetup(void);
	baseSetup( const baseSetup&);
	~baseSetup(void);

	bool Intialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:

	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutDownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	userInput* _userInput;
	grahpics* _graphics;



};


// used to redirect to function in class
static LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

static baseSetup* ApllicationHandle = 0;


#endif