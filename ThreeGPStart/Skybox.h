#pragma once

#include "Model.h"
class Skybox : public Model
{
public:
	Skybox(const Transform& argTransform = Transform(), const std::string& argName = std::string() , const bool argHasLighting = false);

	virtual void Draw(GLuint argProgram, const Helpers::Camera& argCamera, const glm::mat4& argProjection_Xform) const override final;
};

