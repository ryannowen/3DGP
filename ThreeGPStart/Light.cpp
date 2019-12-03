#include "Light.h"

int Light::numOfLights{ 0 };

void Light::SendNumOfLights(GLuint argProgram)
{
	/// Create  ID and then Sends  data to shader as Uniform
	GLuint numOfLights_id = glGetUniformLocation(argProgram, "numOfLights");
	glUniform1i(numOfLights_id, numOfLights);
}

Light::Light(const ELightType argLightType, const Transform argTransform, const float argLightFOV, const glm::vec3 argLightColour, const float argLightRange, const float argLightIntensity)
	:	Renderable(argTransform), light_type(argLightType), light_fov(argLightFOV), light_colour(argLightColour), light_range(argLightRange), light_intensity(argLightIntensity)
{
	lightID = numOfLights;
	numOfLights++;
}

void Light::Draw(GLuint argProgram, const Helpers::Camera& argCamera, Transform argParentTransform) const
{
	Transform newTransform{ currentTransform + argParentTransform };

	GLuint light_type_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(lightID) + "].light_type").c_str());
	glUniform1i(light_type_id, static_cast<int>(light_type));

	GLuint light_position_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(lightID) + "].light_position").c_str());
	glUniform3fv(light_position_id, 1, glm::value_ptr(newTransform.GetPosition()));

	GLuint light_direction_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(lightID) + "].light_direction").c_str());
	glUniform3fv(light_direction_id, 1, glm::value_ptr(newTransform.GetRotation()));

	GLuint light_fov_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(lightID) + "].light_fov").c_str());
	glUniform1f(light_fov_id, light_fov);

	GLuint light_colour_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(lightID) + "].light_colour").c_str());
	glUniform3fv(light_colour_id, 1, glm::value_ptr(light_colour));

	GLuint light_range_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(lightID) + "].light_range").c_str());
	glUniform1f(light_range_id, light_range);

	GLuint light_intensity_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(lightID) + "].light_intensity").c_str());
	glUniform1f(light_intensity_id, light_intensity);

}
