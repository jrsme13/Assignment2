#pragma once

#ifndef _MODEL_H_
#define _MODEL_H_


#include<d3d10.h>
#include<D3DX10math.h>
#include "Texture.h"
#include "ObjectLoader.h"
#include <vector>
using namespace std;



class Model
{
public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialize(ID3D10Device*, char*, WCHAR*);
	void Shutdown();
	void RenderToGraphics(ID3D10Device*);

	int GetIndexCount();
	ID3D10ShaderResourceView* GetTexture();

private:


	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelValues
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VertexLoader// this can be use for normals aswell
	{
		float x,y,z;
	};

	struct Textures
	{
		float tu,tv;
	};

	struct Faces
	{
		int vertexIndex1, textureIndex1, normalIndex1;
		int vertexIndex2, textureIndex2, normalIndex2;
		int vertexIndex3, textureIndex3, normalIndex3;
	};


	bool InitializeBuffers(ID3D10Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D10Device*);

	bool LoadTexture(ID3D10Device*, WCHAR*);
	void ReleaseTexture();
	bool LoadModel(char*);
	void ReleaseModel();


	ID3D10Buffer *_vertexBuffer, *_indexBuffer;
	int _vertexCount, _indexCount;
	Texture* _texture;

	ModelValues* _model;

	vector<ModelValues> _modelArray;


	VertexLoader* _verteciesArray;
	Textures* _texturesArray;
	VertexLoader*	_normalsArray;
	Faces* _facesArray;

};

#endif