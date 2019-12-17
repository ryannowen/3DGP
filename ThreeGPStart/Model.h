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
protected:
	bool hasLighting;
	std::vector<SModelData> subMeshes;

	void CreateGeometry(const Helpers::Mesh& argMesh);
	void CreateTexture(const Helpers::Material& argMat);

public:
	std::vector<Renderable*> children;
	Model(const Transform& argTransform = Transform(), const std::string& argName = std::string(), const bool argHasLighting = true);
	~Model();

	virtual void Draw(GLuint argProgram, const Helpers::Camera& argCamera, const glm::mat4& argProjection_Xform) const override;
	void LoadMesh(const std::string& argModelPath);
	virtual Renderable* FindChild(const std::string& argChildName) override final;
	virtual void CalculateTransform(glm::mat4 argParentTransform, GLuint argProgram) override final;
};

