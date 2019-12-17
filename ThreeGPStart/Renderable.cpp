#include "Renderable.h"

Renderable::Renderable(const Transform argTransform, const std::string& argName)
	: currentTransform(argTransform), name(argName)
{
}
