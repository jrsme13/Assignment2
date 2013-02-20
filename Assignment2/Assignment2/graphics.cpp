#include "graphics.h"


graphics::graphics()
{
	_D3D = 0;
}

graphics::graphics(const graphics& other)
{
}


graphics::~graphics()
{
}

bool graphics::Intialize(int width, int height,HWND hwnd)
{
	bool result;

		
	// Create the Direct3D object.
	_D3D = new Direct3D;
	if(!_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = _D3D->intialize(width, height, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	return true;
}

void graphics::Shutdown()
{
	if(_D3D)
	{
		_D3D->Shutdown();
		delete _D3D;
		_D3D = 0;
	}
	return;
}

bool graphics::Frame()
{
	bool result;


	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

bool graphics::Render()
{
	// Clear the buffers to begin the scene.
	_D3D->SetupScene(255.f,0.f, 0.f, 1.0f);


	// Present the rendered scene to the screen.
	_D3D->DrawScene();

	return true;
}
