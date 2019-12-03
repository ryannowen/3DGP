#pragma once
#include "Renderable.h"

#include "ExternalLibraryHeaders.h"

#include "Mesh.h"

#include "Material.h"


struct SModelData
{
	SModelData(GLuint argVAO, GLuint argMeshElements)
		: VAO(argVAO), meshElements(argMeshElements)
	{}

	GLuint VAO;
	GLuint meshElements;
	std::vector<Material> materials;
};

class Model : public Renderable
{
private:
	bool hasLighting;
	std::vector<SModelData> subMeshes;
	std::vector<Renderable*> children;

public:
	Model();
	virtual void Draw(GLuint argProgram, Helpers::Camera& argCamera) const override;
	void LoadMesh(const std::string& argModelPath);
	void CreateGeometry(const Helpers::Mesh& argMesh);
	void CreateTexture(const Helpers::Material& argMat);
};

