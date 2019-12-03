#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"

#include "ImageLoader.h"

#include <unordered_map>

#include "Renderable.h"

class Terrain;

enum class EMeshType
{
	eSkybox,
	eModel
};

struct MeshLoadData
{
	MeshLoadData(const EMeshType argMeshType, const std::string& argMeshPath, const std::vector<int>& argChildrenIDs, const Transform argRelativeTransform)
		: meshType(argMeshType), meshPath(argMeshPath), childrenIDs(argChildrenIDs), relativeTransform(argRelativeTransform)
	{}

	EMeshType meshType;
	std::string meshPath;
	std::vector<int> childrenIDs;
	Transform relativeTransform;
	bool isCreated{ false };
};

class Renderer
{
private:
	std::vector<Renderable*> meshes;
	std::unordered_map<std::string, Helpers::ImageLoader> textureMap;

	// Program object - to host shaders
	GLuint m_program{ 0 };

	bool CreateProgram();

public:
	Renderer()=default;
	~Renderer();

	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

