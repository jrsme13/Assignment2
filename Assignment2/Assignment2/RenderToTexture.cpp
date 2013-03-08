#include "RenderToTexture.h"


RenderToTexture::RenderToTexture(void)
{

	_renderTargetTexture = 0;
	_renderTargetView = 0;
	_shaderResourceView = 0;
	_depthStencilBuffer = 0;
	_depthStencilView = 0;
}

RenderToTexture::RenderToTexture(const RenderToTexture& other)
{

}


RenderToTexture::~RenderToTexture(void)
{
}


bool RenderToTexture::Initialize(ID3D10Device* device, int width, int height,float screenDepth, float screenNear)
{
	D3D10_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D10_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D10_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D10_TEXTURE2D_DESC depthBufferDesc;
	D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;


	ZeroMemory(&textureDesc,sizeof(textureDesc));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D10_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&textureDesc,NULL , &_renderTargetTexture);

	if(FAILED(result))
	{
		return false;
	}

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;


	result = device->CreateRenderTargetView(_renderTargetTexture, &renderTargetViewDesc, &_renderTargetView);
	if(FAILED(result))
	{
		return false;
	}

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	result = device->CreateShaderResourceView(_renderTargetTexture, &shaderResourceViewDesc, &_shaderResourceView);
	if(FAILED(result))
	{
		return false;
	}

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the viewport for rendering.
    _viewport.Width = width;
    _viewport.Height = height;
    _viewport.MinDepth = 0.0f;
    _viewport.MaxDepth = 1.0f;
    _viewport.TopLeftX = 0;
    _viewport.TopLeftY = 0;

	// Setup the projection matrix.
	D3DXMatrixPerspectiveFovLH(&_projectionMatrix, ((float)D3DX_PI / 4.0f), ((float)width / (float)height), screenNear, screenDepth);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&_orthoMatrix, (float)width, (float)height, screenNear, screenDepth);

	return true;

}

void RenderToTexture::SetRenderTarget(ID3D10Device* device)
{

	device->OMSetRenderTargets(1,&_renderTargetView,_depthStencilView);
	device->RSSetViewports(1, &_viewport);
	return;
}


void RenderToTexture::ClearRenderTarget(ID3D10Device* device, float red, float green, float blue, float alpha)
{

	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	device->ClearRenderTargetView(_renderTargetView,color);

	device->ClearDepthStencilView(_depthStencilView,D3D10_CLEAR_DEPTH, 1.0f, 0);

	return;


}

ID3D10ShaderResourceView* RenderToTexture::GetShaderResourceView()
{

	return _shaderResourceView;
}


void RenderToTexture::Shutdown()
{
	if(_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = 0;
	}

	if(_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = 0;
	}


	if(_shaderResourceView)
	{
		_shaderResourceView->Release();
		_shaderResourceView = 0;
	}

	if(_renderTargetView)
	{
		_renderTargetView->Release();
		_renderTargetView = 0;
	}

	if(_renderTargetTexture)
	{
		_renderTargetTexture->Release();
		_renderTargetTexture = 0;
	}

	return;


}



void RenderToTexture::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = _projectionMatrix;
	return;
}


void RenderToTexture::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = _orthoMatrix;
	return;
}