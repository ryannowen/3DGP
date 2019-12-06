#include "Renderable.h"

Renderable::Renderable(Transform argTransform, Renderable* argParent)
	: currentTransform(argTransform), parent(argParent)
{
}
