#pragma once

#include "Renderer.h"

#include "Model.h"

class Terrain : public Model
{
private:
	void CalculateNormals();
	void ApplyDisplacementMap(const std::string& argDisplacementMapPath, const int argNumOfCellsX, const int argNumOfCellsZ);

public:
	Helpers::Mesh terrainMesh;

	Helpers::ImageLoader texture;

	int xTiling{ 10 }, zTiling{ 10 };
	Terrain();

	void CreateTerrain(const int argNumCellsX, const int argNumCellsZ, const float argSizeX, const float argSizeZ, const int argTextureTilingX, const int argTextureTilingZ, const std::string& argTextureName, const std::string& argDisplacementMapPath = std::string());
};

