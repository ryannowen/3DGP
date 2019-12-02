#pragma once

#include "Transform.h"

class Renderable
{
public:
	Renderable();

	bool disabled;
	Transform currentTransform;
	Transform oldTransform;

	virtual void Draw() const = 0;

};

