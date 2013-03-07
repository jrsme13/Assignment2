#pragma once

#ifndef _RENDERTOTEXTURE_H_
#define _RENDERTOTEXTURE_H_


#include <d3d10.h>

class RenderToTexture
{
public:
	RenderToTexture();
	RenderToTexture(const RenderToTexture&);
	~RenderToTexture();


	bool Initialize(ID3D10Device*, int, int);
	void Shutdown();

	void SetRenderTarget(ID3D10Device*, ID3D10DepthStencilView*);
	void ClearRenderTarget(ID3D10Device*, ID3D10DepthStencilView*, float, float, float, float);
	ID3D10ShaderResourceView* GetShaderResourceView();

private:

	ID3D10Texture2D* _renderTargetTexture;
	ID3D10RenderTargetView* _renderTargetView;
	ID3D10ShaderResourceView* _shaderResourceView;
};

#endif