#pragma once

#include "Model.h"
class Skybox : public Model
{

	virtual void Draw(GLuint argProgram, const Helpers::Camera& argCamera, Transform argParentTransform) const override final;
};

