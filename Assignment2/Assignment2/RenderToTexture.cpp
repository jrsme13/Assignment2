#include "RenderToTexture.h"


RenderToTexture::RenderToTexture(void)
{

	_renderTargetTexture = 0;
	_renderTargetView = 0;
	_shaderResourceView = 0;
}

RenderToTexture::RenderToTexture(const RenderToTexture& other)
{

}


RenderToTexture::~RenderToTexture(void)
{
}


bool RenderToTexture::Initialize(ID3D10Device* device, int width, int height)
{
	D3D10_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D10_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D10_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;


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

	return true;

}

void RenderToTexture::SetRenderTarget(ID3D10Device* device, ID3D10DepthStencilView* depthStencilView)
{

	device->OMSetRenderTargets(1,&_renderTargetView,depthStencilView);

	return;
}


void RenderToTexture::ClearRenderTarget(ID3D10Device* device, ID3D10DepthStencilView* depthStencilView, float red, float green, float blue, float alpha)
{

	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	device->ClearRenderTargetView(_renderTargetView,color);

	device->ClearDepthStencilView(depthStencilView,D3D10_CLEAR_DEPTH, 1.0f, 0);

	return;


}

ID3D10ShaderResourceView* RenderToTexture::GetShaderResourceView()
{

	return _shaderResourceView;
}


void RenderToTexture::Shutdown()
{
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