#pragma once

#include "Transform.h"

#include "Camera.h"

class Renderable
{
public:
	Renderable();

	bool disabled;
	Transform currentTransform;
	Transform oldTransform;

	virtual void Draw(GLuint argProgram, Helpers::Camera& argCamera) const = 0;

};

