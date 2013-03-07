#pragma once

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "Direct3D.h"
#include "camera.h"
#include "Model.h"
#include "shader.h"
#include "Lights.h"


//global

const bool FULL_SCREEN = false;
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

	
	Direct3D* _D3D;
	camera* _camera;
	Model* _model;
	Model* _model2;
	shader* _shader;
	Texture* _texture;
	Lights *_light;
	Lights *_light2;

	bool Render(float);
};


#endif