#include "Terrain.h"

Terrain::Terrain(const int argNumCellsX, const int argNumCellsZ, const float argSizeX, const float argSizeZ, const int argTextureTilingX, const int argTextureTilingZ, const std::string& argTextureFilePath, const std::string& argDisplacementMapPath, const glm::mat4 argTransform)
	: texturePath(argTextureFilePath), xform(argTransform)
{
	CreateTerrain(argNumCellsX, argNumCellsZ, argSizeX, argSizeZ, argTextureTilingX, argTextureTilingZ, texturePath, argDisplacementMapPath);
}

void Terrain::CreateTerrain(const int argNumCellsX, const int argNumCellsZ, const float argSizeX, const float argSizeZ, const int argTextureTilingX, const int argTextureTilingZ, const std::string& argTextureFilePath, const std::string& argDisplacementMapPath)
{
	xTiling = argTextureTilingX;
	zTiling = argTextureTilingZ;

	int numOfCellsX{ argNumCellsX };
	int numOfCellsZ{ argNumCellsZ };

	unsigned char* heightData{ NULL };

	if (argDisplacementMapPath != std::string())
	{
		if (texture.Load(argDisplacementMapPath))
		{
			numOfCellsX = texture.Width() - 1;
			numOfCellsZ = texture.Height() - 1;
			heightData = (unsigned char*)texture.GetData();
		}
	}

	int numOfVertsX{ numOfCellsX + 1 };
	int numOfVertsZ{ numOfCellsZ + 1 };

	float vertexXSpacing{ argSizeX / static_cast<float>(numOfVertsX) };
	float vertexZSpacing{ argSizeZ / static_cast<float>(numOfVertsZ) };

	float worldPositionX{ -argSizeX / 2.0f };
	float worldPositionZ{ argSizeZ / 2.0f };

	float height{ 0.0f };

	/// Sets vertex locations and applies initial upward facing normals
	for (int z = 0; z < numOfVertsZ; z++)
	{
		for (int x = 0; x < numOfVertsX; x++)
		{
			if (argDisplacementMapPath != std::string())
				height = heightData[0];
			else
				height = 0.0f;

			if (height < 0)
				height = height;

			terrainMesh.vertices.push_back(glm::vec3(worldPositionX + (x * vertexXSpacing), height, worldPositionZ + (-z * vertexZSpacing)));


			terrainMesh.uvCoords.push_back(glm::vec2(x / static_cast<float>(numOfVertsX) * xTiling, z / static_cast<float>(numOfVertsZ) * zTiling));
			terrainMesh.normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

			heightData += 4;
		}
	}

	/// Creates Diamond Grid Elements
	bool diamondToggle{ true };
	for (int z = 0; z < numOfCellsZ; z++)
	{
		for (int x = 0; x < numOfCellsX; x++)
		{
			int startVert{ z * numOfVertsX + x };

			if (diamondToggle)
			{
				terrainMesh.elements.push_back(startVert);
				terrainMesh.elements.push_back(startVert + 1);
				terrainMesh.elements.push_back(startVert + numOfVertsX);

				terrainMesh.elements.push_back(startVert + 1);
				terrainMesh.elements.push_back(startVert + numOfVertsX + 1);
				terrainMesh.elements.push_back(startVert + numOfVertsX);
			}
			else
			{
				terrainMesh.elements.push_back(startVert);
				terrainMesh.elements.push_back(startVert + numOfVertsX + 1);
				terrainMesh.elements.push_back(startVert + numOfVertsX);

				terrainMesh.elements.push_back(startVert);
				terrainMesh.elements.push_back(startVert + 1);
				terrainMesh.elements.push_back(startVert + numOfVertsX + 1);
			}

			diamondToggle = !diamondToggle;
		}

		diamondToggle = !diamondToggle;
	}

	/// Updates Vertex Normals
	for (int i = 0; i < terrainMesh.elements.size(); i += 3)
	{
		glm::vec3 edge1{ terrainMesh.vertices[terrainMesh.elements[i + 1]] - terrainMesh.vertices[terrainMesh.elements[i]] };
		glm::vec3 edge2{ terrainMesh.vertices[terrainMesh.elements[i + 2]] - terrainMesh.vertices[terrainMesh.elements[i]] };

		glm::vec3 normal{ glm::cross(edge1, edge2) };

		terrainMesh.normals[terrainMesh.elements[i]] += normal;
		terrainMesh.normals[terrainMesh.elements[i + 1]] += normal;
		terrainMesh.normals[terrainMesh.elements[i + 2]] += normal;
	}

	for (glm::vec3& normal : terrainMesh.normals)
		normal = glm::normalize(normal);
}

