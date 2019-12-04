#pragma once

#include "Model.h"
class Skybox : public Model
{
public:
	Skybox(const Transform& argTransform = Transform(), const bool argHasLighting = false);

	virtual void Draw(GLuint argProgram, const Helpers::Camera& argCamera, const glm::mat4& argProjection_Xform, glm::mat4 argParentTransform) const override final;
};

