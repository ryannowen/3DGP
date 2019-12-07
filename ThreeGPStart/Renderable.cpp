#include "Renderable.h"

Renderable::Renderable(const Transform argTransform, Renderable* argParent, const std::string& argName)
	: currentTransform(argTransform), parent(argParent), name(argName)
{
}
