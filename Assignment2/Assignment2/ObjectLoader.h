#pragma once

#ifndef _OBJECLOADER_H_
#define _OBJECTLOADER_H_


#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


class ObjectLoader
{
public:
	ObjectLoader();
	ObjectLoader(const ObjectLoader&);
	~ObjectLoader();


	void Initialize(char* file);


	int getFace(int i);

private:



	struct Vertex// this can be use for normals aswell
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

	vector<Vertex> _verteciesArray;
	vector<Textures> _texturesArray;
	vector<Vertex>	_normalsArray;
	vector<Faces> _facesArray;

	
};

#endif

