#pragma once

#ifndef _MODEL_H_
#define _MODEL_H_


#include<d3d10.h>
#include<D3DX10math.h>

class Model
{
public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialize(ID3D10Device*);
	void Shutdown();
	void RenderToGraphics(ID3D10Device*);

	int GetIndexCount();


private:


	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

	bool InitializeBuffers(ID3D10Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D10Device*);

	ID3D10Buffer *_vertexBuffer, *_indexBuffer;
	int _vertexCount, _indexCount;

};

#endif