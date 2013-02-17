#pragma once

#ifndef _DIRECT3D_H_
#define _DIRECT3D_H_


//linking to librarys

#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"dxgi.lib")


#include <d3d10.h>
#include <D3DX10.h>

class Direct3D
{
public:
	Direct3D();
	Direct3D();
	~Direct3D();
};

#endif