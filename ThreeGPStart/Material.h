#pragma once

#include "ExternalLibraryHeaders.h"

class Material
{	

public:
	Material(const glm::vec3 argAmbientColour = glm::vec3(0), const glm::vec3 argSpecularColour = glm::vec3(1), const float argSpecularIntensity = 0, const GLuint argTextureRef = 0);
	
	glm::vec3 ambient_Colour;
	glm::vec3 specular_Colour;
	float specular_Intensity;
	GLuint textureRef;


	
	//void ApplyMaterial();
};

