#pragma once

#include "ExternalLibraryHeaders.h"

class Transform
{
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

public:
	Transform(glm::vec3 argPosition = glm::vec3(0), glm::vec3 argRotation = glm::vec3(0), glm::vec3 argScale = glm::vec3(1))
		: position(argPosition), rotation(argRotation), scale(argScale) 
	{}

	void SetPosition(glm::vec3 argNewPosition) { position = argNewPosition; }
	void AddPosition(glm::vec3 argPosition) { position += argPosition; }
	glm::vec3 GetPosition() const { return position; }

	void SetRotation(glm::vec3 argNewRotation) { rotation = argNewRotation; }
	void AddRotation(glm::vec3 argAngle) { rotation += argAngle; if (rotation.x > 360) rotation.x -= 360; if (rotation.y > 360) rotation.y -= 360; if (rotation.z > 360) rotation.z -= 360; }
	glm::vec3 GetRotation() const { return rotation; }

	void SetScale(glm::vec3 argNewScale) { scale = argNewScale; }
	void AddScale(glm::vec3 argScale) { scale += argScale; }
	glm::vec3 GetScale() const { return scale; }

	inline void operator+=(const Transform& argTrans)
	{
		position += argTrans.position;
		rotation += argTrans.rotation;
		scale += argTrans.scale;
	}
};

inline Transform operator+(Transform lhs, const Transform& rhs)
{
	lhs += rhs;

	return lhs;
}