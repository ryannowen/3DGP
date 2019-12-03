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
	Material material;
};

class Model : public Renderable
{
private:
	void CreateGeometry(const Helpers::Mesh& argMesh);
	void CreateTexture(const Helpers::Material& argMat);

protected:
	bool hasLighting;
	std::vector<SModelData> subMeshes;
	std::vector<Renderable*> children;

public:
	Model(const Transform& argTransform = Transform(), const bool argHasLighting = true);
	virtual void Draw(GLuint argProgram, const Helpers::Camera& argCamera, Transform argParentTransform) const override;
	void LoadMesh(const std::string& argModelPath);

};

