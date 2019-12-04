#include "Material.h"

Material::Material(const glm::vec3 argAmbientColour, const glm::vec3 argSpecularColour, const float argSpecularIntensity, const GLuint argTextureRef)
	: ambient_Colour(argAmbientColour), specular_Colour(argSpecularColour), specular_Intensity(argSpecularIntensity), textureRef(argTextureRef)
{
}

void Material::ApplyMaterial(GLuint argProgram) const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureRef);

	/// Sends Texture Sample to shader
	GLuint texSample_id = glGetUniformLocation(argProgram, "texSample");
	glUniform1i(texSample_id, 0);

	GLuint ambient_Colour_id = glGetUniformLocation(argProgram, "material.ambient_Colour");
	glUniform3fv(ambient_Colour_id, 1, glm::value_ptr(ambient_Colour));

	GLuint specular_Colour_id = glGetUniformLocation(argProgram, "material.specular_Colour");
	glUniform3fv(specular_Colour_id, 1, glm::value_ptr(specular_Colour));

	GLuint specular_Intensity_id = glGetUniformLocation(argProgram, "material.specular_Intensity");
	glUniform1f(specular_Intensity_id, specular_Intensity);
}
