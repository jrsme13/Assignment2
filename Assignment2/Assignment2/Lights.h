#pragma once

#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include <d3dx10math.h>

class Lights
{
public:
	Lights();
	Lights(const Lights&);
	~Lights();

	void SetAmbient(float,float,float,float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColour(float,float,float,float);
	void SetSpecularPower(float);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR4 GetAmbient();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetSpecularColor();
	float		GetSpecularPower();

private:
	D3DXVECTOR4 _diffuseColor;
	D3DXVECTOR3 _direction;
	D3DXVECTOR4 _ambient;
	D3DXVECTOR4 _specColour;
	float		_specPower;

};

#endif