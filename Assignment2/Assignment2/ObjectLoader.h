#pragma once

#ifndef _OBJECLOADER_H_
#define _OBJECTLOADER_H_


#include <iostream>
#include <fstream>
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
		int vertexIndex1, vertexIndex2, vertexIndex3;
		int textureIndex1, textureIndex2, textureIndex3;
		int normalIndex1, normalIndex2, normalIndex3;
	};

	
};

#endif

