#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"

#include "ImageLoader.h"

#include <unordered_map>

#include "Renderable.h"

class Terrain;

struct MeshLoadData
{
	MeshLoadData(const std::string& argMeshPath, const int argMeshParentID, const Transform argRelativeTransform)
		: meshPath(argMeshPath), meshParentID(argMeshParentID), relativeTransform(argRelativeTransform)
	{}

	std::string meshPath;
	int meshParentID;
	Transform relativeTransform;
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

