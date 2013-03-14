#include "Model.h"
#include "Direct3D.h"


Model::Model()
{

	_vertexBuffer = 0;
	_indexBuffer = 0;
	_texture = 0;
	_model = 0;
	/*_verteciesArray = 0;
	_texturesArray = 0;
	_normalsArray = 0;
	_facesArray = 0;*/
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

	
	vertices = new Vertex[_vertexCount];
	if(!vertices)
	{
		return false;
	}

	
	indices = new unsigned long[_indexCount];
	if(!indices)
	{
		return false;
	}

	// load in the model infromation into the vertices
	for(int i=0; i<_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(_modelArray[i].x, _modelArray[i].y, _modelArray[i].z);
		vertices[i].texture = D3DXVECTOR2(_modelArray[i].tu, _modelArray[i].tv);
		vertices[i].normal = D3DXVECTOR3(_modelArray[i].nx, _modelArray[i].ny, _modelArray[i].nz);

		indices[i] = i;
	}

	
	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	
	vertexData.pSysMem = vertices;

	
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	
	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	
	indexData.pSysMem = indices;

	
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	
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

	ifstream ifs;
	char data;


	int vertexCount = 0;
	int textureCount = 0;
	int normalsCount = 0;
	int faceCount = 0;

	ifs.open(file);

	if(ifs.fail()== true)
	{
		return false;
	}

	ifs.get(data);
	while(!ifs.eof())
	{
		if(data == 'v')
		{
			ifs.get(data);

			if(data == ' ')
			{ 
				vertexCount++; 
			}
			if(data == 't') 
			{ 
				textureCount++;
			}
			if(data == 'n') 
			{
				normalsCount++; 
			}
		}

		// If the line starts with 'f' then increment the face count.
		if(data == 'f')
		{
			ifs.get(data);
			if(data == ' ') 
			{ 
				faceCount++;
			}
		}
		
		// Otherwise read in the remainder of the line.
		while(data != '\n')
		{
			ifs.get(data);
		}

		// Start reading the beginning of the next line.
		ifs.get(data);
	}

	// Close the file.
	ifs.close();

	

	for(int i = 0; i < vertexCount; i++)
	{
		VertexLoader vertecies;
		_verteciesArray.push_back(vertecies);
	}


	

	for(int i = 0; i < textureCount; i++)
	{
		Textures tex;
		_textureArray.push_back(tex);
	}
	
	
	for(int i = 0; i <	normalsCount; i++)
	{
		VertexLoader normal;
		_normalsArray.push_back(normal);
	}
	

	for(int i = 0; i <	faceCount; i++)
	{
		Faces f;
		_facesArray.push_back(f);
	}




	ifs.open(file);

	vertexCount = 0;
	textureCount = 0;
	normalsCount = 0;
	faceCount = 0;

	ifs.get(data);
	while(!ifs.eof())
	{
		

		if(data == 'v')
		{
			ifs.get(data);

			if(data ==' ')
			{
				ifs >> _verteciesArray[vertexCount].x >> _verteciesArray[vertexCount].y >> _verteciesArray[vertexCount].z;

				_verteciesArray[vertexCount].z = _verteciesArray[vertexCount].z * -1.0f;
				vertexCount++;
			}

			if(data == 't')
			{

				ifs>>_textureArray[textureCount].tu>>_textureArray[textureCount].tv;
				_textureArray[textureCount].tv = 1.0f - _textureArray[textureCount].tv;

				textureCount++;

			}

			if(data =='n')
			{
				ifs>>_normalsArray[normalsCount].x>>_normalsArray[normalsCount].y>>_normalsArray[normalsCount].z;

				_normalsArray[normalsCount].z = _normalsArray[normalsCount].z *-1.0f;

				normalsCount++;
			}
		}


		if(data == 'f')
		{
			ifs.get(data);

			if(data == ' ')
			{
				char forwardslash;

				//read in backwards

				ifs>>_facesArray[faceCount].vertexIndex3>>forwardslash>>_facesArray[faceCount].textureIndex3>>forwardslash>>_facesArray[faceCount].normalIndex3
					>>_facesArray[faceCount].vertexIndex2>>forwardslash>>_facesArray[faceCount].textureIndex2>>forwardslash>>_facesArray[faceCount].normalIndex2
					>>_facesArray[faceCount].vertexIndex1>>forwardslash>>_facesArray[faceCount].textureIndex1>>forwardslash>>_facesArray[faceCount].normalIndex1;

				faceCount++;

			}

		}

		while(data !='\n')
		{

			ifs.get(data);
		}

		ifs.get(data);


	}

	int modelCount = 0;

	_model = new ModelValues[faceCount*3];
	if(!_model)
	{
		return false;
	}

	for(int i = 0; i < faceCount*3; i++)
	{
		ModelValues model;
		_modelArray.push_back(model);
	}

	ifs.close();

	_vertexCount = faceCount*3;
	_indexCount =  _vertexCount;
	int vIndex, tIndex, nIndex;

	for(int i=0; i<faceCount; i++)
	{
		vIndex = _facesArray[i].vertexIndex1 - 1;
		tIndex = _facesArray[i].textureIndex1 - 1;
		nIndex = _facesArray[i].normalIndex1 - 1;

		_modelArray[modelCount].x = _verteciesArray[vIndex].x;
		_modelArray[modelCount].y = _verteciesArray[vIndex].y;
		_modelArray[modelCount].z = _verteciesArray[vIndex].z;

		_modelArray[modelCount].tu = _textureArray[tIndex].tu;
		_modelArray[modelCount].tv = _textureArray[tIndex].tv;


		_modelArray[modelCount].nx = _normalsArray[nIndex].x;
		_modelArray[modelCount].ny = _normalsArray[nIndex].y;
		_modelArray[modelCount].nz = _normalsArray[nIndex].z;

		modelCount++;
		vIndex = _facesArray[i].vertexIndex2 - 1;
		tIndex = _facesArray[i].textureIndex2 - 1;
		nIndex = _facesArray[i].normalIndex2 - 1;

		_modelArray[modelCount].x = _verteciesArray[vIndex].x;
		_modelArray[modelCount].y = _verteciesArray[vIndex].y;
		_modelArray[modelCount].z = _verteciesArray[vIndex].z;

		_modelArray[modelCount].tu = _textureArray[tIndex].tu;
		_modelArray[modelCount].tv = _textureArray[tIndex].tv;


		_modelArray[modelCount].nx = _normalsArray[nIndex].x;
		_modelArray[modelCount].ny = _normalsArray[nIndex].y;
		_modelArray[modelCount].nz = _normalsArray[nIndex].z;

		modelCount++;
		vIndex = _facesArray[i].vertexIndex3 - 1;
		tIndex = _facesArray[i].textureIndex3 - 1;
		nIndex = _facesArray[i].normalIndex3 - 1;

		_modelArray[modelCount].x = _verteciesArray[vIndex].x;
		_modelArray[modelCount].y = _verteciesArray[vIndex].y;
		_modelArray[modelCount].z = _verteciesArray[vIndex].z;

		_modelArray[modelCount].tu = _textureArray[tIndex].tu;
		_modelArray[modelCount].tv = _textureArray[tIndex].tv;


		_modelArray[modelCount].nx = _normalsArray[nIndex].x;
		_modelArray[modelCount].ny = _normalsArray[nIndex].y;
		_modelArray[modelCount].nz = _normalsArray[nIndex].z;

		modelCount++;
	}


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