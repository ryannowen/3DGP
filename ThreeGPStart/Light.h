#pragma once
#include "Renderable.h"

enum class ELightType
{
	eDirectional,
	ePoint,
	eSpot
};

class Light : public Renderable
{
public:
	static int numOfLights;
	static void SendNumOfLights(GLuint argProgram);

	Light(const ELightType argLightType, const Transform argTransform, const float argLightFOV = 5.0f, const glm::vec3 argLightColour = glm::vec3(1), const float argLightRange = 200.0f, const float argLightIntensity = 10.0f);

	ELightType light_type;
	float light_fov;
	glm::vec3 light_colour;
	float light_range;
	float light_intensity;

	virtual void Draw(GLuint argProgram, const Helpers::Camera& argCamera, Transform argParentTransform) const override final;
};

