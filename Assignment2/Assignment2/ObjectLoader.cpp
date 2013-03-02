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
	char line[256];


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

			if(data ==' ')
			{
				ifs>>_verteciesArray[vertexCount].x>>_verteciesArray[vertexCount].y>>_verteciesArray[vertexCount].z;

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

	ifs.close();
}