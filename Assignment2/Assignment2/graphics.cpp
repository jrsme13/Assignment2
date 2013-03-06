#include "graphics.h"


graphics::graphics()
{
	_D3D = 0;
	_camera = 0;
	_model = 0;
	_model2 = 0;
	_shader = 0;
	//_texture = 0;
	_light = 0;
	_light2 = 0;
	
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
	_camera->SetPosition(0.0f, 10.0f, -12.0f);
	_camera->SetRotation(45.0f,0.0f,0.0f);
	// Create the model object.
	_model = new Model;
	if(!_model)
	{
		return false;
	}

	// Initialize the model object.
	result = _model->Initialize(_D3D->GetDevice(),"../Assignment2/plane.obj",L"../Assignment2/grey.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	_model2 = new Model;
	if(!_model2)
	{
		return false;
	}

	result = _model2->Initialize(_D3D->GetDevice(),"../Assignment2/sphere.obj",L"../Assignment2/seafloor.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model2 object.", L"Error", MB_OK);
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

	// Create the light object.
	_light = new Lights;
	if(!_light)
	{
		return false;
	}

	_light->SetAmbient(0.05f, 0.05f, 0.05f, 1.0f);
	_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	_light->SetDirection(1.0f, -1.0f, 1.0f);
	_light->SetSpecularColour(0.0f, 0.0f, 0.0f, 1.0f);
	_light->SetSpecularPower(2.0f);


	_light2 = new Lights;
	if(!_light2)
	{
		return false;
	}

	_light2->SetAmbient(0.15f, 0.15f, 0.15f, 1.0f);
	_light2->SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	_light2->SetDirection(-1.0f, -1.0f, 1.0f);
	_light2->SetSpecularColour(0.0f, 0.0f, 0.0f, 1.0f);
	_light2->SetSpecularPower(32.0f);
	

	// Initialize the light object.
	

	return true;
}

void graphics::Shutdown()
{


	// Release the light object.
	if(_light)
	{
		delete _light;
		_light = 0;
	}

	if(_light2)
	{
		delete _light2;
		_light2 = 0;
	}
	

	// Release the texture shader object.
	/*if(_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = 0;
	}*/


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

	//if(_model2)
	//{
	//	_model2->Shutdown();
	//	delete _model2;
	//	_model2 = 0;
	//}

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
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.0001f;
	if(rotation > 360.0f)
	{
		rotation = -365.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}

bool graphics::Render(float rotation)
{
	// Clear the buffers to begin the scene.
	//_D3D->SetupScene(255.f,0.f, 0.f, 1.0f);

	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix,tempA,tempB,tempC, temphold;
	

	// Clear the buffers to begin the scene.
	_D3D->SetupScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	_camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	_camera->GetViewMatrix(viewMatrix);
	_D3D->GetWorldMatrix(worldMatrix);
	_D3D->GetProjectionMatrix(projectionMatrix);

	
	D3DXMatrixRotationX(&worldMatrix, 90.0f);
	

	_model2->RenderToGraphics(_D3D->GetDevice());
	_shader->Render(_D3D->GetDevice(), _model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,_model2->GetTexture(),
		_light->GetDirection(), _light->GetDiffuseColor(),_light->GetAmbient(),_camera->GetPosition(),_light->GetSpecularColor(),_light->GetSpecularPower(),_light2->GetDirection(),_light2->GetDiffuseColor()
		,_light2->GetSpecularColor(),_light2->GetSpecularPower());
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	_model->RenderToGraphics(_D3D->GetDevice());

	D3DXMatrixTranslation(&tempA,0.0f,1.0f,1.0f);
	D3DXMatrixRotationX(&tempB, 90.0f);

	D3DXMatrixMultiply(&worldMatrix,&tempA,&tempB);
	D3DXMatrixScaling(&tempC,2.0f,2.0f,2.0f);

	D3DXMatrixMultiply(&worldMatrix,&worldMatrix,&tempC);

	// Render the model using the color shader.
	_shader->Render(_D3D->GetDevice(), _model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,_model->GetTexture(),
		_light->GetDirection(), _light->GetDiffuseColor(),_light->GetAmbient(),_camera->GetPosition(),_light->GetSpecularColor(),_light->GetSpecularPower(),_light2->GetDirection(),_light2->GetDiffuseColor()
		,_light2->GetSpecularColor(),_light2->GetSpecularPower());

	


	

	// Present the rendered scene to the screen.
	_D3D->DrawScene();

	return true;
}
