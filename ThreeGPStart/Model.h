#pragma once
#include "Renderable.h"

#include "ExternalLibraryHeaders.h"

#include "Material.h"

struct SModelData
{
	std::vector<Material> materials;
	GLuint VAO;
	GLuint meshElements;
};

class Model : public Renderable
{

public:
	Model();
	virtual void Draw() const override;

};
