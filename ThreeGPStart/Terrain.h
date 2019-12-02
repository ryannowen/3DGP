#pragma once

#include "Renderer.h"

#include "Model.h"

class Terrain : public Model
{
public:
	Helpers::Mesh terrainMesh;
	glm::mat4 xform;

	std::string texturePath;
	Helpers::ImageLoader texture;

	int xTiling{ 10 }, zTiling{ 10 };
	Terrain(const int argNumCellsX, const int argNumCellsZ, const float argSizeX, const float argSizeZ, const int argTextureTilingX, const int argTextureTilingZ, const std::string& argTextureFilePath, const std::string& argDisplacementMapPath = std::string(), const glm::mat4 argTransform = glm::mat4(1));

	void CreateTerrain(const int argNumCellsX, const int argNumCellsZ, const float argSizeX, const float argSizeZ, const int argTextureTilingX, const int argTextureTilingZ, const std::string& argTextureFilePath, const std::string& argDisplacementMapPath = std::string());
	void CalculateNormals();
	void ApplyDisplacementMap(const std::string& argDisplacementMapPath, const int argNumOfCellsX, const int argNumOfCellsZ);

	virtual void Draw() const override final;
};

