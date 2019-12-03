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

	ELightType light_type;
	float light_fov;
	glm::vec3 light_colour;
	float light_range;
	float light_intensity;

	virtual void Draw(GLuint argProgram, Helpers::Camera& argCamera) const override final;
};

