#pragma once

#include "Transform.h"

#include "Camera.h"

class Renderable
{
protected:
	std::string name;
	glm::mat4 transform{ glm::mat4(1) };

public:
	Renderable(const Transform argTransform, const std::string& argName);
	Transform currentTransform;

	const std::string& GetName() const { return name; };

	virtual void Draw(GLuint argProgram, const Helpers::Camera& argCamera, const glm::mat4& argView_Xform) const = 0;
	virtual void CalculateTransform(glm::mat4 argParentTransform, GLuint argProgram) = 0;
	virtual Renderable* FindChild(const std::string& argChildName);
};

