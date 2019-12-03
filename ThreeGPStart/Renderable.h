#pragma once

#include "Transform.h"

#include "Camera.h"

class Renderable
{
protected:
	bool disabled{ false };
	Transform currentTransform;
	Transform oldTransform;

public:
	Renderable(Transform argTransform);

	virtual void Draw(GLuint argProgram, const Helpers::Camera& argCamera, Transform argParentTransform) const = 0;

};

