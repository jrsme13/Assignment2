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
	_renderTexture = 0;
	_depthShader = 0;
	_renderTexture2 = 0;
	
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
	result = _model->Initialize(_D3D->GetDevice(),"../Assignment2/plane.obj",L"../Assignment2/stone01.dds");
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

	result = _model2->Initialize(_D3D->GetDevice(),"../Assignment2/new_teapotOBJ.obj",L"../Assignment2/stone01.dds");
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

	_depthShader = new DepthShader;
	if(!_depthShader)
	{
		return false;
	}

	result = _depthShader->Initialize(_D3D->GetDevice(),hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the depthshader object.", L"Error", MB_OK);
		return false;
	}


	// Create the light object.
	_light = new Lights;
	if(!_light)
	{
		return false;
	}

	_light->SetAmbient(0.15f, 0.15f, 0.3f, 1.0f);
	_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	_light->SetSpecularColour(0.0f, 0.0f, 0.0f, 1.0f);
	_light->SetSpecularPower(32.0f);
	_light->SetPosition(-2.0f,4.0f,2.0f);
	_light->GenerateProjectionMatrix(SCREEN_DEPTH,SCREEN_NEAR);


	_light2 = new Lights;
	if(!_light2)
	{
		return false;
	}

	_light2->SetAmbient(0.15f, 0.15f, 0.15f, 1.0f);
	_light2->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	_light2->SetSpecularColour(0.0f, 0.0f, 0.0f, 1.0f);
	_light2->SetSpecularPower(32.0f);
	_light2->SetPosition(2.0f,3.0f,-2.0f);
	_light2->GenerateProjectionMatrix(SCREEN_DEPTH,SCREEN_NEAR);
	

	// Initialize the light object.

	_renderTexture = new RenderToTexture;
	if(!_renderTexture)
	{
		return false;
	}

	result = _renderTexture->Initialize(_D3D->GetDevice(),SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Graphics Could not Intialize Render To Texture", L"Error", MB_OK);
		return false;
	}

	_renderTexture2 = new RenderToTexture;
	if(!_renderTexture2)
	{
		return false;
	}

	// Initialize the second render to texture object.
	result = _renderTexture2->Initialize(_D3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the second render to texture object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void graphics::Shutdown()
{


	// Release the light object.
	if(_renderTexture2)
	{
		_renderTexture2->Shutdown();
		delete _renderTexture2;
		_renderTexture2 = 0;
	}


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
	
	if(_renderTexture)
	{
		_renderTexture->Shutdown();
		delete _renderTexture;
		_renderTexture = 0;
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

	if(_depthShader)
	{
		_depthShader->Shutdown();
		delete _depthShader;
		_depthShader = 0;
	}

	// Release the model object.
	if(_model)
	{
		_model->Shutdown();
		delete _model;
		_model = 0;
	}

	if(_model2)
	{
		_model2->Shutdown();
		delete _model2;
		_model2 = 0;
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

	static float lightPositionX = -2.0f;
	
	// Render the graphics scene.
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.0001f;
	if(rotation > 360.0f)
	{
		rotation = -365.0f;
	}



	lightPositionX += 0.00005f;
	if(lightPositionX > -0.5f)
	{
		lightPositionX = -2.0f;
	}

	_light->SetPosition(-2.0f,4.0f,lightPositionX);

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
	bool result;

	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix,tempA,tempB,tempC,temphold;
	D3DXMATRIX lightViewMatrix, lightProjectionMatrix;
	D3DXMATRIX lightViewMatrix2, lightProjectionMatrix2;
	static float rotation = 90.0f;
	
	result = RenderSceneToTexTure();
	if(!result)
	{
		return false;

	}

	result = RenderSceneToTexTure2();
	if(!result)
	{
		return false;
	}
	// Clear the buffers to begin the scene.
	_D3D->SetupScene(0.0f, 0.0f, 0.0f, 1.0f);


	_camera->Render();

	_light->GenerateViewMatrix();
	_light2->GenerateViewMatrix();
	// Get the world, view, and projection matrices from the camera and d3d objects.
	_camera->GetViewMatrix(viewMatrix);
	_D3D->GetWorldMatrix(worldMatrix);
	_D3D->GetProjectionMatrix(projectionMatrix);

	_light->GetViewMatrix(lightViewMatrix);
	_light->GetProjectionMatrix(lightProjectionMatrix);

	
	_light2->GetViewMatrix(lightViewMatrix2);
	_light2->GetProjectionMatrix(lightProjectionMatrix2);

	//D3DXMatrixRotationX(&worldMatrix, 90.0f);
	

	_model2->RenderToGraphics(_D3D->GetDevice());

	//_depthShader->Render(_D3D->GetDevice(), _model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	_shader->Render(_D3D->GetDevice(), _model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, 
			       lightProjectionMatrix,_model2->GetTexture(),_renderTexture->GetShaderResourceView(),
				   _light->GetPosition(), _light->GetDiffuseColor(),_light->GetAmbient(),_camera->GetPosition(),_light->GetSpecularColor(),_light->GetSpecularPower(),_light2->GetPosition(),_light2->GetDiffuseColor()
				   ,_light2->GetSpecularColor(),_light2->GetSpecularPower(),lightViewMatrix2, lightProjectionMatrix2,_renderTexture2->GetShaderResourceView());
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	_model->RenderToGraphics(_D3D->GetDevice());

	D3DXMatrixTranslation(&tempA,0.0f,1.0f,1.0f);
	D3DXMatrixRotationX(&tempB, 90.0f);

	D3DXMatrixMultiply(&worldMatrix,&tempA,&tempB);
	D3DXMatrixScaling(&tempC,2.0f,2.0f,2.0f);

	D3DXMatrixMultiply(&worldMatrix,&worldMatrix,&tempC);

	//_depthShader->Render(_D3D->GetDevice(), _model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);

	// Render the model using the color shader.
	_shader->Render(_D3D->GetDevice(), _model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,lightViewMatrix, 
		lightProjectionMatrix,_model->GetTexture(),_renderTexture->GetShaderResourceView(),_light->GetPosition(), _light->GetDiffuseColor(),_light->GetAmbient(),_camera->GetPosition(),_light->GetSpecularColor(),_light->GetSpecularPower(),_light2->GetPosition(),_light2->GetDiffuseColor()
		,_light2->GetSpecularColor(),_light2->GetSpecularPower(),lightViewMatrix2, lightProjectionMatrix2,_renderTexture2->GetShaderResourceView());

	
	

	_D3D->DrawScene();
	

	return true;
}


bool graphics::RenderSceneToTexTure()
{
	D3DXMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix, translateMatrix,tempA,tempB,tempC;

	_renderTexture->SetRenderTarget(_D3D->GetDevice());

	_renderTexture->ClearRenderTarget(_D3D->GetDevice(),0.0f,0.0f,1.0f,1.0f);

	_light->GenerateViewMatrix();

	_D3D->GetWorldMatrix(worldMatrix);

	// Get the view and orthographic matrices from the light object.
	_light->GetViewMatrix(lightViewMatrix);
	_light->GetProjectionMatrix(lightProjectionMatrix);

	_model2->RenderToGraphics(_D3D->GetDevice());
	_depthShader->Render(_D3D->GetDevice(), _model2->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);

	

	D3DXMatrixTranslation(&tempA,0.0f,1.0f,0.5f);
	D3DXMatrixRotationX(&tempB, 90.0f);

	D3DXMatrixMultiply(&worldMatrix,&tempA,&tempB);
	D3DXMatrixScaling(&tempC,2.0f,2.0f,2.0f);

	D3DXMatrixMultiply(&worldMatrix,&worldMatrix,&tempC);
	_model->RenderToGraphics(_D3D->GetDevice());
	_depthShader->Render(_D3D->GetDevice(), _model->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);

	

	_D3D->SetBackBufferRenderTarget();
	
	_D3D->ResetViewport();

	return true;

}

bool graphics::RenderSceneToTexTure2()
{
	D3DXMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix, translateMatrix,tempA,tempB,tempC;
	float posX, posY, posZ;


	// Set the render target to be the render to texture.
	_renderTexture2->SetRenderTarget(_D3D->GetDevice());

	// Clear the render to texture.
	_renderTexture2->ClearRenderTarget(_D3D->GetDevice(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the light view matrix based on the light's position.
	_light2->GenerateViewMatrix();

	// Get the world matrix from the d3d object.
	_D3D->GetWorldMatrix(worldMatrix);

	// Get the view and orthographic matrices from the light object.
	_light2->GetViewMatrix(lightViewMatrix);
	_light2->GetProjectionMatrix(lightProjectionMatrix);

	// Setup the translation matrix for the cube model.
	_model2->RenderToGraphics(_D3D->GetDevice());
	_depthShader->Render(_D3D->GetDevice(), _model2->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);

	

	D3DXMatrixTranslation(&tempA,0.0f,1.0f,0.5f);
	D3DXMatrixRotationX(&tempB, 90.0f);

	D3DXMatrixMultiply(&worldMatrix,&tempA,&tempB);
	D3DXMatrixScaling(&tempC,2.0f,2.0f,2.0f);

	D3DXMatrixMultiply(&worldMatrix,&worldMatrix,&tempC);
	_model->RenderToGraphics(_D3D->GetDevice());
	_depthShader->Render(_D3D->GetDevice(), _model->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);

	// Render the cube model with the depth shader.
	

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	_D3D->ResetViewport();

	return true;
}


//void graphics::RenderScene()
//{
//	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix,tempA,tempB,tempC;
//	D3DXMATRIX lightViewMatrix, lightProjectionMatrix;
//	static float rotation = 90.0f;
//
//
//	_camera->Render();
//
//	_light->GenerateViewMatrix();
//
//	// Get the world, view, and projection matrices from the camera and d3d objects.
//	_camera->GetViewMatrix(viewMatrix);
//	_D3D->GetWorldMatrix(worldMatrix);
//	_D3D->GetProjectionMatrix(projectionMatrix);
//
//	_light->GetViewMatrix(lightViewMatrix);
//	_light->GetProjectionMatrix(lightProjectionMatrix);
//
//	
//	D3DXMatrixRotationX(&worldMatrix, 90.0f);
//	
//
//	_model2->RenderToGraphics(_D3D->GetDevice());
//
//	//_depthShader->Render(_D3D->GetDevice(), _model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
//	_shader->Render(_D3D->GetDevice(), _model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, 
//			       lightProjectionMatrix,_model2->GetTexture(),_renderTexture->GetShaderResourceView(),
//		_light->GetDirection(), _light->GetDiffuseColor(),_light->GetAmbient(),_camera->GetPosition(),_light->GetSpecularColor(),_light->GetSpecularPower(),_light2->GetDirection(),_light2->GetDiffuseColor()
//		,_light2->GetSpecularColor(),_light2->GetSpecularPower());
//	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
//	_model->RenderToGraphics(_D3D->GetDevice());
//
//	D3DXMatrixTranslation(&tempA,0.0f,1.0f,1.0f);
//	D3DXMatrixRotationX(&tempB, 90.0f);
//
//	D3DXMatrixMultiply(&worldMatrix,&tempA,&tempB);
//	D3DXMatrixScaling(&tempC,2.0f,2.0f,2.0f);
//
//	D3DXMatrixMultiply(&worldMatrix,&worldMatrix,&tempC);
//
//	//_depthShader->Render(_D3D->GetDevice(), _model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
//
//	// Render the model using the color shader.
//	_shader->Render(_D3D->GetDevice(), _model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,lightViewMatrix, 
//			       lightProjectionMatrix,_model->GetTexture(),_renderTexture->GetShaderResourceView(),_light->GetDirection(), _light->GetDiffuseColor(),_light->GetAmbient(),_camera->GetPosition(),_light->GetSpecularColor(),_light->GetSpecularPower(),_light2->GetDirection(),_light2->GetDiffuseColor()
//		,_light2->GetSpecularColor(),_light2->GetSpecularPower());
//
//	return;
//}
