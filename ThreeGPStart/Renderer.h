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

enum class ERenderableType
{
	eSkybox,
	eModel,
	eTerrain,
	eLight
};

struct SMeshLoadData
{
	SMeshLoadData(const ERenderableType argRenderableType, const std::string& argMeshPath, const std::vector<int>& argChildrenIDs, const Transform argRelativeTransform)
		: argRenderableType(argRenderableType), meshPath(argMeshPath), childrenIDs(argChildrenIDs), relativeTransform(argRelativeTransform)
	{}

	ERenderableType argRenderableType;
	std::string meshPath;
	std::vector<int> childrenIDs;
	Transform relativeTransform;
	bool isCreated{ false };
};

struct STerrainLoadData : public SMeshLoadData
{
	STerrainLoadData(const ERenderableType argRenderableType, const Transform argRelativeTransform, const int argNumCellsX = 50, const int argNumCellsZ = 50, const float argSizeX = 1000.0f, const float argSizeZ = 1000.0f, const int argTextureTilingX = 1, const int argTextureTilingZ = 1, const std::string& argTextureFilePath = "Data\\Textures\\MissingTexture.jpg", const std::string& argDisplacementMapPath = std::string())
		: SMeshLoadData(argRenderableType, std::string(), std::vector<int>(), argRelativeTransform),
		numCellsX(argNumCellsX), numCellsZ(argNumCellsZ), sizeX(argSizeX), sizeZ(argSizeZ), textureTilingX(argTextureTilingX), textureTilingZ(argTextureTilingZ), textureFilePath(argTextureFilePath), displacementMapPath(argDisplacementMapPath)
	{}

	int numCellsX, numCellsZ, textureTilingX, textureTilingZ;
	std::string textureFilePath;
	float sizeX, sizeZ;
	std::string displacementMapPath;
};

struct SLightLoadData : public SMeshLoadData
{
	SLightLoadData(const ERenderableType argRenderableType, const Transform argRelativeTransform, const ELightType argLightType, const float argLightFOV, const glm::vec3 argLightColour, const float argLightRange, const float argLightIntensity)
		: SMeshLoadData(argRenderableType, std::string(), std::vector<int>(), argRelativeTransform),
		light_type(argLightType), light_fov(argLightFOV), light_colour(argLightColour), light_range(argLightRange), light_intensity(argLightIntensity)
	{}

	ELightType light_type;
	float light_fov;
	glm::vec3 light_colour;
	float light_range;
	float light_intensity;

};

class Renderer
{
private:

	std::vector<SMeshLoadData*> modelInfomation;
	


	std::unordered_map<std::string, Helpers::ImageLoader> textureMap;

	// Program object - to host shaders
	GLuint m_program{ 0 };

	bool CreateProgram();
	
	void CreateMesh(SMeshLoadData* argLoadData, std::vector<Renderable*>& argMeshLocation);

public:
	Renderer()=default;
	~Renderer();
	std::vector<Renderable*> renderables;
	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

