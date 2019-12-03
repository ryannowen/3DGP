#include "Material.h"

Material::Material(const glm::vec3 argAmbientColour, const glm::vec3 argSpecularColour, const float argSpecularIntensity, const GLuint argTextureRef)
	: ambient_Colour(argAmbientColour), specular_Colour(argSpecularColour), specular_Intensity(argSpecularIntensity), textureRef(argTextureRef)
{
}
