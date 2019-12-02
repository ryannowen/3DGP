#pragma once

#include "ExternalLibraryHeaders.h"

class Material
{
public:
	glm::vec3 ambient_Colour;
	glm::vec3 specular_Colour;
	float specular_Intensity;
	GLuint textureRef;

	//void ApplyMaterial();
};

