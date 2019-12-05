#pragma once

#include "Transform.h"

#include "Camera.h"

class Renderable
{
protected:
	bool disabled{ false };

	Transform oldTransform;

public:
	Renderable(Transform argTransform);
	//~Renderable();

	Transform currentTransform;
	virtual void Draw(GLuint argProgram, const Helpers::Camera& argCamera, const glm::mat4& argView_Xform, glm::mat4 argParentTransform) const = 0;

};

