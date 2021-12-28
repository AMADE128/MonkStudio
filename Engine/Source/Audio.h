#pragma once

#include "Resource.h"
#include <string>

typedef unsigned char GLubyte;

struct AudioParameters
{

};

class Audio : public Resource
{
public:
	Audio(uint uid, std::string& assets, std::string& library);
	Audio(uint uid, std::string& library);

	~Audio();

	void Load() override;
	void UnLoad() override;


private: 
	unsigned int id;
	GLubyte* data;

	AudioParameters parameters;
};