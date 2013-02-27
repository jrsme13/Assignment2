#include "Model.h"
#include "Direct3D.h"


Model::Model()
{

	_vertexBuffer = 0;
	_indexBuffer = 0;
	_texture = 0;
}

Model::Model(const Model& other)
{

	
}


Model::~Model()
{
}


bool Model::Initialize(ID3D10Device* device,WCHAR* textureFile)
{
	bool result;


	
	result = InitializeBuffers(device);
	if(!result)
	{
		
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFile);
	if(!result)
	{
		return false;
	}

	return true;
}


void Model::Shutdown()
{
	ReleaseTexture();	
	ShutdownBuffers();

	return;
}

void Model::RenderToGraphics(ID3D10Device* device)
{
	
	RenderBuffers(device);

	return;
}

int Model::GetIndexCount()
{
	return _indexCount;
}


ID3D10ShaderResourceView* Model::GetTexture()
{
	return _texture->GetTexture();
}

bool Model::InitializeBuffers(ID3D10Device* device)
{
	Vertex* vertices;
	unsigned long* indices;
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	_vertexCount = 3;
	_indexCount = 3;


	// Create the vertex array.
	vertices = new Vertex[_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[_indexCount];
	if(!indices)
	{
		return false;
	}


	// Load the vertex array with data.
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);


	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void Model::ShutdownBuffers()
{
	// Release the index buffer.
	if(_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = 0;
	}

	return;
}


void Model::RenderBuffers(ID3D10Device* device)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(Vertex); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	device->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	device->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Model::LoadTexture(ID3D10Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	_texture = new Texture;
	if(!_texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = _texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void Model::ReleaseTexture()
{
	// Release the texture object.
	if(_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = 0;
	}

	return;
}