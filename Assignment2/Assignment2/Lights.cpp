#include "Lights.h"


Lights::Lights(void)
{
}

Lights::Lights(const Lights& other)
{
}


Lights::~Lights(void)
{
}


void Lights::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void Lights::SetAmbient(float red, float green, float blue, float alpha)
{
	_ambient = D3DXVECTOR4(red,green,blue,alpha);
	return;

}


void Lights::SetSpecularColour(float red, float green, float blue, float alpha)
{

	_specColour = D3DXVECTOR4(red,green,blue,alpha);
	return;
}

void Lights::SetSpecularPower(float power)
{
	_specPower = power;
	return;
}

void Lights::SetPosition(float x, float y, float z)
{
	_position = D3DXVECTOR3(x,y,z);
	return;
}

void Lights::SetDirection(float x, float y, float z)
{
	_direction = D3DXVECTOR3(x, y, z);
	return;
}


D3DXVECTOR4 Lights::GetDiffuseColor()
{
	return _diffuseColor;
}



D3DXVECTOR3 Lights::GetDirection()
{
	return _direction;
}

D3DXVECTOR4 Lights::GetAmbient()
{
	return _ambient;

}

D3DXVECTOR4 Lights::GetSpecularColor()
{
	return _specColour;

}


float Lights::GetSpecularPower()
{

	return _specPower;
}

D3DXVECTOR3 Lights::GetPosition()
{

	return _position;
}

void Lights::GenerateViewMatrix()
{
	D3DXVECTOR3 up;
	D3DXVECTOR3 lookAt;

	lookAt.x =	 0.0f;
	lookAt.y =	 0.0f;
	lookAt.z =	 0.0f;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//_position = -_direction * 2;

	// Create the view matrix from the three vectors.
	D3DXMatrixLookAtLH(&_viewMatrix, &_position, &lookAt, &up);
	
	return;
}

void Lights::GenerateProjectionMatrix(float screenDepth, float screenNear)
{
	float fieldOfView, screenAspect;


	// Setup field of view and screen aspect for a square light source.
	fieldOfView = (float)D3DX_PI / 2.0f;
	screenAspect = 1.0f;

	// Create the projection matrix for the light.
	//D3DXMatrixOrthoLH(&_projectionMatrix, 30.0f, 30.0f, screenNear, screenDepth);
	D3DXMatrixPerspectiveFovLH(&_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	return;
}

void Lights::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = _viewMatrix;
	return;
}


void Lights::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = _projectionMatrix;
	return;
}