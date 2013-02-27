#include "Texture.h"


Texture::Texture(void)
{
	_texture = 0;
}

Texture::Texture(const Texture& other)
{
	
}


Texture::~Texture(void)
{
}


bool Texture::Initialize(ID3D10Device* device, WCHAR* filename)
{
	HRESULT result;


	// Load the texture in.
	result = D3DX10CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &_texture, NULL);
	if(FAILED(result))
	{

		return false;
	}

	return true;
}


void Texture::Shutdown()
{
	// Release the texture resource.
	if(_texture)
	{
		_texture->Release();
		_texture = 0;
	}

	return;
}


ID3D10ShaderResourceView* Texture::GetTexture()
{
	return _texture;
}