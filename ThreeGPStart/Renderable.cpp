#include "Renderable.h"

Renderable::Renderable(const Transform argTransform, const std::string& argName)
	: currentTransform(argTransform), name(argName)
{
}

/// Used for when the renderable cannot have children (E.g Light)
Renderable* Renderable::FindChild(const std::string& argChildName)
{
	return nullptr;
}
