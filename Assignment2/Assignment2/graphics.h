#pragma once

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "Direct3D.h"


//global

const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class graphics
{
public:
	graphics();
	graphics(const graphics&);
	~graphics();


	bool Intialize(int width,int height,HWND);
	void Shutdown();
	bool Frame();

private:

	bool Render();
	Direct3D* _D3D;
};


#endif