#pragma once



class ObjectLoader
{
public:
	ObjectLoader(void);
	ObjectLoader(const ObjectLoader&);
	~ObjectLoader(void);


	void Initialize(char* file);
};

