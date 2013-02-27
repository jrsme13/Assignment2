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