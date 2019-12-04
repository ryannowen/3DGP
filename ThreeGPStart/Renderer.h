#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"

#include "ImageLoader.h"

#include <unordered_map>

#include "Renderable.h"
#include "Light.h"
#include "Terrain.h"

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




	std::vector<MeshLoadData> modelInfomation
	{
		MeshLoadData(EMeshType::eSkybox, "Data\\Sky\\Hills\\skybox.x", std::vector<int>(), Transform()),
	/*	MeshLoadData(EMeshType::eModel, "Data\\Models\\AquaPig\\hull.obj", std::vector<int>{2, 3, 4, 5}, Transform(glm::vec3(150, 0, 0), glm::vec3(0, 0, 0), glm::vec3(150))),
			MeshLoadData(EMeshType::eModel, "Data\\Models\\AquaPig\\wing_right.obj", std::vector<int>(), Transform(glm::vec3(-2.231, 0.272, -2.663), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1))),
			MeshLoadData(EMeshType::eModel, "Data\\Models\\AquaPig\\wing_left.obj", std::vector<int>(), Transform(glm::vec3(2.231, 0.272, -2.663), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1))),
			MeshLoadData(EMeshType::eModel, "Data\\Models\\AquaPig\\propeller.obj", std::vector<int>(), Transform(glm::vec3(0, 1.395, -3.616), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1))),
			MeshLoadData(EMeshType::eModel, "Data\\Models\\AquaPig\\gun_base.obj", std::vector<int>{6}, Transform(glm::vec3(0, 0.569, -1.866), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1))),
				MeshLoadData(EMeshType::eModel, "Data\\Models\\AquaPig\\gun.obj", std::vector<int>(), Transform(glm::vec3(0, 1.506, 0.644), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1))),*/
		MeshLoadData(EMeshType::eModel, "Data\\Models\\Jeep\\jeep.obj", std::vector<int>(), Transform(glm::vec3(0, 0, 0), glm::vec3(1.5, 0, 0), glm::vec3(1)))
	};





	std::vector<Renderable*> meshes;
	std::unordered_map<std::string, Helpers::ImageLoader> textureMap;

	// Program object - to host shaders
	GLuint m_program{ 0 };

	bool CreateProgram();
	
	void CreateMesh(MeshLoadData& argLoadData, std::vector<Renderable*>& argMeshLocation);

public:
	Renderer()=default;
	~Renderer();

	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

