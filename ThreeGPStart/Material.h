#pragma once

#include "ExternalLibraryHeaders.h"

class Material
{	

public:
	Material(const glm::vec3 argAmbientColour, const glm::vec3 argSpecularColour, const float argSpecularIntensity, const GLuint argTextureRef);
	
	glm::vec3 ambient_Colour;
	glm::vec3 specular_Colour;
	float specular_Intensity;
	GLuint textureRef;


	
	//void ApplyMaterial();
};

