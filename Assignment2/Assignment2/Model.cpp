#include "Model.h"
#include "Direct3D.h"


Model::Model()
{

	_vertexBuffer = 0;
	_indexBuffer = 0;
	_texture = 0;
	_model = 0;
}

Model::Model(const Model& other)
{

	
}


Model::~Model()
{
}


bool Model::Initialize(ID3D10Device* device,char* modelFile,WCHAR* textureFile)
{
	bool result;



	result = LoadModel(modelFile);
	if(!result)
	{
		return false;
	}

	
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
	ReleaseModel();
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
	// Load the vertex array and index array with data.
	for(int i=0; i<_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(_model[i].x, _model[i].y, _model[i].z);
		vertices[i].texture = D3DXVECTOR2(_model[i].tu, _model[i].tv);
		vertices[i].normal = D3DXVECTOR3(_model[i].nx, _model[i].ny, _model[i].nz);

		indices[i] = i;
	}

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


bool Model::LoadModel(char* file)
{
	/*ObjectLoader loader;
	int faces;
	int faceCount = 0;
	int modelCount = 0;

	loader.Initialize(file);

	faces = loader.NumberOfFaces();

	_vertexCount = faces*3;
	_indexCount =  _vertexCount;

	_model = new ModelValues[faces*3];
	if(!_model)
	{
		return false;
	}

	while( faceCount < faces)
	{
		for(int i = 0; i < 3; i++)
		{
			_model[modelCount].x = loader.GetFaceVertexX(faceCount,i);
			_model[modelCount].y = loader.GetFaceVertexY(faceCount,i);
			_model[modelCount].z = loader.GetFaceVertexZ(faceCount,i);

			_model[modelCount].tu = loader.GetFaceTextureTu(faceCount,i);
			_model[modelCount].tv = loader.GetFaceTextureTV(faceCount,i);

			_model[modelCount].nx = loader.GetFaceNormalX(faceCount,i);
			_model[modelCount].ny = loader.GetFaceNormalY(faceCount,i);
			_model[modelCount].nz = loader.GetFaceNormalZ(faceCount,i);

			modelCount++;
		}

		faceCount++;
	}

	return true;*/

	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(file);
	
	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> _vertexCount;

	// Set the number of indices to be the same as the vertex count.
	_indexCount = _vertexCount;

	// Create the model using the vertex count that was read in.
	_model = new ModelValues[_vertexCount];
	if(!_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<_vertexCount; i++)
	{
		fin >> _model[i].x >> _model[i].y >> _model[i].z;
		fin >> _model[i].tu >> _model[i].tv;
		fin >> _model[i].nx >> _model[i].ny >> _model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}






void Model::ReleaseModel()
{
	if(_model)
	{
		delete [] _model;
		_model = 0;
	}

	return;
}