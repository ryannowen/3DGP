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

	virtual void Draw(GLuint argProgram, const Helpers::Camera& argCamera, const glm::mat4& argView_Xform, glm::mat4 argParentTransform) const = 0;

};

