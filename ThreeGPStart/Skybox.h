#pragma once

#include "Model.h"
class Skybox : public Model
{
public:
	Skybox(const Transform& argTransform = Transform(), const bool argHasLighting = true);

	virtual void Draw(GLuint argProgram, const Helpers::Camera& argCamera, Transform argParentTransform) const override final;
};

