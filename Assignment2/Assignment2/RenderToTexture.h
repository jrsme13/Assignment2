#pragma once

#ifndef _RENDERTOTEXTURE_H_
#define _RENDERTOTEXTURE_H_


#include <d3d10.h>
#include <d3dx10math.h>


class RenderToTexture
{
public:
	RenderToTexture();
	RenderToTexture(const RenderToTexture&);
	~RenderToTexture();


	bool Initialize(ID3D10Device*, int, int, float, float);
	void Shutdown();

	void SetRenderTarget(ID3D10Device*);
	void ClearRenderTarget(ID3D10Device*, float, float, float, float);
	ID3D10ShaderResourceView* GetShaderResourceView();

	void GetProjectionMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

private:

	ID3D10Texture2D* _renderTargetTexture;
	ID3D10RenderTargetView* _renderTargetView;
	ID3D10ShaderResourceView* _shaderResourceView;

	ID3D10Texture2D* _depthStencilBuffer;
	ID3D10DepthStencilView* _depthStencilView;
	D3D10_VIEWPORT _viewport;
	D3DXMATRIX _projectionMatrix;
	D3DXMATRIX _orthoMatrix;
};

#endif