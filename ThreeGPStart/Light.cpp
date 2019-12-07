#include "ExternalLibraryHeaders.h"

#include "glm/gtx/matrix_decompose.hpp"
#include "Light.h"


int Light::numOfLights{ 0 };

void Light::SendNumOfLights(GLuint argProgram)
{
	/// Create  ID and then Sends  data to shader as Uniform
	GLuint numOfLights_id = glGetUniformLocation(argProgram, "numOfLights");
	glUniform1i(numOfLights_id, numOfLights);

	numOfLights = 0;
}

Light::Light(Renderable* argParent, const std::string& argName, const ELightType argLightType, const Transform argTransform, const float argLightFOV, const glm::vec3 argLightColour, const float argLightRange, const float argLightIntensity)
	:	Renderable(argTransform, argParent, argName), light_type(argLightType), light_fov(argLightFOV), light_colour(argLightColour), light_range(argLightRange), light_intensity(argLightIntensity)
{	
}

void Light::ApplyLight(GLuint argProgram)
{
	if (disabled)
	{
		std::cout << name << " disabled, not applying" << std::endl;
		return;
	}



	Renderable* currentParent{ this };
	glm::mat4 transform = glm::mat4(1);

	while (currentParent != NULL)
	{
		/// Adds Current transform to matrix
		transform = glm::translate(transform, currentParent->currentTransform.GetPosition());

		transform = glm::rotate(transform, glm::radians(currentParent->currentTransform.GetRotation().x), glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, glm::radians(currentParent->currentTransform.GetRotation().y), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(currentParent->currentTransform.GetRotation().z), glm::vec3(0, 0, 1));

		transform = glm::scale(transform, currentParent->currentTransform.GetScale());

		currentParent = currentParent->parent;
	}

	/// Decompresses matrix (Function requires all parameters)
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(transform, scale, rotation, translation, skew, perspective);
	rotation = glm::conjugate(rotation);


	GLuint light_type_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_type").c_str());
	glUniform1i(light_type_id, static_cast<int>(light_type));

	GLuint light_position_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_position").c_str());
	glUniform3fv(light_position_id, 1, glm::value_ptr(translation));

	GLuint light_direction_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_direction").c_str());
	glUniform3fv(light_direction_id, 1, glm::value_ptr(glm::vec3(rotation.x, -rotation.y, rotation.z)));

	GLuint light_fov_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_fov").c_str());
	glUniform1f(light_fov_id, light_fov);

	GLuint light_colour_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_colour").c_str());
	glUniform3fv(light_colour_id, 1, glm::value_ptr(light_colour));

	GLuint light_range_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_range").c_str());
	glUniform1f(light_range_id, light_range);

	GLuint light_intensity_id = glGetUniformLocation(argProgram, std::string("lights[" + std::to_string(numOfLights) + "].light_intensity").c_str());
	glUniform1f(light_intensity_id, light_intensity);
	
	numOfLights++;
}

void Light::Draw(GLuint argProgram, const Helpers::Camera& argCamera, const glm::mat4& argProjection_Xform, glm::mat4 argParentTransform) const
{
}
