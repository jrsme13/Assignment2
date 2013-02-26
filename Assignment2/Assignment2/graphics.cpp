#include "graphics.h"


graphics::graphics()
{
	_D3D = 0;
	_camera = 0;
	_model = 0;
	_shader = 0;
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

	// Create the camera object.
	_camera = new camera;
	if(!_camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	_camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Create the model object.
	_model = new Model;
	if(!_model)
	{
		return false;
	}

	// Initialize the model object.
	result = _model->Initialize(_D3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	_shader = new shader;
	if(!_shader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = _shader->Initialize(_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}


	return true;
}

void graphics::Shutdown()
{

	// Release the color shader object.
	if(_shader)
	{
		_shader->Shutdown();
		delete _shader;
		_shader = 0;
	}

	// Release the model object.
	if(_model)
	{
		_model->Shutdown();
		delete _model;
		_model = 0;
	}

	// Release the camera object.
	if(_camera)
	{
		delete _camera;
		_camera = 0;
	}


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

	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;


	// Clear the buffers to begin the scene.
	_D3D->SetupScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	_camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	_camera->GetViewMatrix(viewMatrix);
	_D3D->GetWorldMatrix(worldMatrix);
	_D3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	_model->RenderToGraphics(_D3D->GetDevice());

	// Render the model using the color shader.
	_shader->Render(_D3D->GetDevice(), _model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);

	// Present the rendered scene to the screen.
	_D3D->DrawScene();

	return true;
}
