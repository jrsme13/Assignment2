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