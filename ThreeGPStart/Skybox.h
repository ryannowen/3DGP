#pragma once

#include "Model.h"
class Skybox : public Model
{

	virtual void Draw(GLuint argProgram, Helpers::Camera& argCamera) const override final;
};

