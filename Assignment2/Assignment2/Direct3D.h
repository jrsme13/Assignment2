#pragma once

#ifndef _DIRECT3D_H_
#define _DIRECT3D_H_


//linking to librarys

#pragma comment(lib,"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3d10.lib")
#pragma comment(lib,"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3dx10.lib")
#pragma comment(lib,"dxgi.lib")


#include <d3d10.h>
#include <D3DX10.h>

class Direct3D
{
public:
	Direct3D();
	Direct3D(const Direct3D&);
	~Direct3D();

	bool intialize(int, int,bool,HWND,bool,float,float);
	void Shutdown();

	void SetupScene(float,float,float,float);
	void DrawScene();

	ID3D10Device* GetDevice();

	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	ID3D10DepthStencilView* GetDepthStencilView();

	void SetBackBufferRenderTarget();

private:

	bool _vsync_enabled;
	IDXGISwapChain* _swapChain;
	ID3D10Device* _device;
	ID3D10RenderTargetView* _renderTargetView;
	ID3D10Texture2D* _depthStencilBuffer;
	ID3D10DepthStencilState* _depthStencilState;
	ID3D10DepthStencilView* _depthStencilView;
	ID3D10RasterizerState* _rasterState;
	D3DXMATRIX _projectionMatrix;
	D3DXMATRIX _worldMatrix;
	D3DXMATRIX _orthoMatrix;

};

#endif