#include "Light.h"

int Light::numOfLights{ 0 };

void Light::SendNumOfLights(GLuint argProgram)
{
	/// Create  ID and then Sends  data to shader as Uniform
	GLuint numOfLights_id = glGetUniformLocation(argProgram, "numOfLights");
	glUniform1i(numOfLights_id, numOfLights);
}

void Light::Draw(GLuint argProgram, Helpers::Camera& argCamera) const
{
	numOfLights++;

	GLuint light_type_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_type").c_str());
	glUniform1i(light_type_id, static_cast<int>(light_type));

	GLuint light_position_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_position").c_str());
	glUniform3fv(light_position_id, 1, glm::value_ptr(currentTransform.GetPosition()));

	GLuint light_direction_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_direction").c_str());
	glUniform3fv(light_direction_id, 1, glm::value_ptr(currentTransform.GetRotation()));

	GLuint light_fov_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_fov").c_str());
	glUniform1f(light_fov_id, light_fov);

	GLuint light_colour_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_colour").c_str());
	glUniform3fv(light_colour_id, 1, glm::value_ptr(light_colour));

	GLuint light_range_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_range").c_str());
	glUniform1f(light_range_id, light_range);

	GLuint light_intensity_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_intensity").c_str());
	glUniform1f(light_intensity_id, light_intensity);
}
