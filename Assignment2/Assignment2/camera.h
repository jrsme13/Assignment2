#pragma once


#ifndef _CAMERA_H_
#define _CAMERA_H_


#include <d3dx10math.h>


class camera
{

public:
	camera();
	camera(const camera&);
	~camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

private:

	float _positionX, _positionY, _positionZ;
	float _rotationX, _rotationY, _rotationZ;
	D3DXMATRIX _viewMatrix;
};

#endif