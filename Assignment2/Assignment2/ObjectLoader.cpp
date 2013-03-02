#include "ObjectLoader.h"


ObjectLoader::ObjectLoader()
{
	
}

ObjectLoader::ObjectLoader(const ObjectLoader& other)
{
}


ObjectLoader::~ObjectLoader()
{
}


void ObjectLoader::Initialize(char* file)
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
		return;
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

	_verteciesArray = new Vertex[vertexCount];
	_texturesArray = new Textures[textureCount];
	_normalsArray = new Vertex[normalsCount];
	_facesArray = new Faces[faceCount];



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

				ifs>>_texturesArray[textureCount].tu>>_texturesArray[textureCount].tv;
				_texturesArray[textureCount].tv = 1.0f - _texturesArray[textureCount].tv;

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

	_faceCount = faceCount;

	ifs.close();

	ofstream fout;


	int vIndex, tIndex, nIndex;
	// Open the output file.
	fout.open("model.txt");
	
	// Write out the file header that our model format uses.
	fout << "Vertex Count: " << (faceCount * 3) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	// Now loop through all the faces and output the three vertices for each face.
	for(int i=0; i<faceCount; i++)
	{
		vIndex = _facesArray[i].vertexIndex1 - 1;
		tIndex = _facesArray[i].textureIndex1 - 1;
		nIndex = _facesArray[i].normalIndex1 - 1;

		fout << _verteciesArray[vIndex].x << ' ' << _verteciesArray[vIndex].y << ' ' << _verteciesArray[vIndex].z << ' '
			<< _texturesArray[tIndex].tu << ' ' << _texturesArray[tIndex].tv << ' '
		     << _normalsArray[nIndex].x << ' ' << _normalsArray[nIndex].y << ' ' << _normalsArray[nIndex].z << endl;

		vIndex = _facesArray[i].vertexIndex2 - 1;
		tIndex = _facesArray[i].textureIndex2 - 1;
		nIndex = _facesArray[i].normalIndex2 - 1;

		fout << _verteciesArray[vIndex].x << ' ' << _verteciesArray[vIndex].y << ' ' << _verteciesArray[vIndex].z << ' '
		     << _texturesArray[tIndex].tu << ' ' << _texturesArray[tIndex].tv << ' '
		     << _normalsArray[nIndex].x << ' ' << _normalsArray[nIndex].y << ' ' << _normalsArray[nIndex].z << endl;

		vIndex = _facesArray[i].vertexIndex3 - 1;
		tIndex = _facesArray[i].textureIndex3 - 1;
		nIndex = _facesArray[i].normalIndex3 - 1;

		fout << _verteciesArray[vIndex].x << ' ' << _verteciesArray[vIndex].y << ' ' << _verteciesArray[vIndex].z << ' '
		     << _texturesArray[tIndex].tu << ' ' << _texturesArray[tIndex].tv << ' '
		     << _normalsArray[nIndex].x << ' ' << _normalsArray[nIndex].y << ' ' << _normalsArray[nIndex].z << endl;
	}

	// Close the output file.
	fout.close();
}


float ObjectLoader::GetFaceVertexX(int face,int faceindex)
{
	int vertexIndex;
	
	if(faceindex == 0)
	{
		vertexIndex = _facesArray[face].vertexIndex1;
	}

	if(faceindex == 1)
	{
		vertexIndex = _facesArray[face].vertexIndex2;
	}

	if(faceindex == 2)
	{
		vertexIndex = _facesArray[face].vertexIndex3;
	}

	return _verteciesArray[vertexIndex].x;
}

float ObjectLoader::GetFaceVertexY(int face,int faceindex)
{
	int vertexIndex;
	
	if(faceindex == 0)
	{
		vertexIndex = _facesArray[face].vertexIndex1;
	}

	if(faceindex == 1)
	{
		vertexIndex = _facesArray[face].vertexIndex2;
	}

	if(faceindex == 2)
	{
		vertexIndex = _facesArray[face].vertexIndex3;
	}

	return _verteciesArray[vertexIndex].y;
}

float ObjectLoader::GetFaceVertexZ(int face,int faceindex)
{
	int vertexIndex;
	
	if(faceindex == 0)
	{
		vertexIndex = _facesArray[face].vertexIndex1;
	}

	if(faceindex == 1)
	{
		vertexIndex = _facesArray[face].vertexIndex2;
	}

	if(faceindex == 2)
	{
		vertexIndex = _facesArray[face].vertexIndex3;
	}

	return _verteciesArray[vertexIndex].z;
}


float ObjectLoader::GetFaceTextureTu(int face, int faceIndex)
{
	int textureIndex;
	
	if(faceIndex == 0)
	{
		textureIndex = _facesArray[face].textureIndex1;
	}

	if(faceIndex == 1)
	{
		textureIndex = _facesArray[face].textureIndex2;
	}

	if(faceIndex == 2)
	{
		textureIndex = _facesArray[face].textureIndex3;
	}

	return _texturesArray[textureIndex].tu;

}

float ObjectLoader::GetFaceTextureTV(int face, int faceIndex)
{
	int textureIndex;
	
	if(faceIndex == 0)
	{
		textureIndex = _facesArray[face].textureIndex1;
	}

	if(faceIndex == 1)
	{
		textureIndex = _facesArray[face].textureIndex2;
	}

	if(faceIndex == 2)
	{
		textureIndex = _facesArray[face].textureIndex3;
	}

	return _texturesArray[textureIndex].tv;

}


float ObjectLoader::GetFaceNormalX(int faces, int faceIndex)
{

	int normalIndex;

	if(faceIndex == 0)
	{
		normalIndex = _facesArray[faces].normalIndex1;
	}

	if(faceIndex == 1)
	{
		normalIndex = _facesArray[faces].normalIndex2;
	}

	if(faceIndex == 2)
	{
		normalIndex = _facesArray[faces].normalIndex3;
	}

	return _normalsArray[normalIndex].x;
}

float ObjectLoader::GetFaceNormalY(int faces, int faceIndex)
{

	int normalIndex;

	if(faceIndex == 0)
	{
		normalIndex = _facesArray[faces].normalIndex1;
	}

	if(faceIndex == 1)
	{
		normalIndex = _facesArray[faces].normalIndex2;
	}

	if(faceIndex == 2)
	{
		normalIndex = _facesArray[faces].normalIndex3;
	}

	return _normalsArray[normalIndex].y;
}

float ObjectLoader::GetFaceNormalZ(int faces, int faceIndex)
{

	int normalIndex;

	if(faceIndex == 0)
	{
		normalIndex = _facesArray[faces].normalIndex1;
	}

	if(faceIndex == 1)
	{
		normalIndex = _facesArray[faces].normalIndex2;
	}

	if(faceIndex == 2)
	{
		normalIndex = _facesArray[faces].normalIndex3;
	}

	return _normalsArray[normalIndex].z;
}


int ObjectLoader::NumberOfFaces()
{

	return _faceCount;

}