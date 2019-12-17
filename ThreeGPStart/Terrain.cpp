#include "Terrain.h"

#include "PerlinNoise.h"

Terrain::Terrain(const Transform& argTransform, const std::string& argName, const bool argHasLighting)
	: Model(argTransform, argName, argHasLighting)
{
}

void Terrain::CreateTerrain(const int argNumCellsX, const int argNumCellsZ, const float argSizeX, const float argSizeZ, const int argTextureTilingX, const int argTextureTilingZ, const std::string& argTextureName, const std::string& argDisplacementMapPath)
{
	int numOfCellsX{ argNumCellsX };
	int numOfCellsZ{ argNumCellsZ };

	int numOfVertsX{ numOfCellsX + 1 };
	int numOfVertsZ{ numOfCellsZ + 1 };

	float vertexXSpacing{ argSizeX / static_cast<float>(numOfVertsX) };
	float vertexZSpacing{ argSizeZ / static_cast<float>(numOfVertsZ) };

	float worldPositionX{ -argSizeX / 2.0f };
	float worldPositionZ{ argSizeZ / 2.0f };

	/// Sets vertex locations and applies initial upward facing normals
	for (int z = 0; z < numOfVertsZ; z++)
	{
		for (int x = 0; x < numOfVertsX; x++)
		{
			terrainMesh.vertices.push_back(glm::vec3(worldPositionX + (x * vertexXSpacing), 0, worldPositionZ + (-z * vertexZSpacing)));

			/// Applies initial vert height using perlin noise
			glm::vec3 currentVert{ terrainMesh.vertices.back() };
			currentVert.y = PerlinNoise::Perlin(currentVert.x, currentVert.z) * 5;

			terrainMesh.uvCoords.push_back(glm::vec2(x / static_cast<float>(numOfCellsX) * argTextureTilingX, z / static_cast<float>(numOfCellsZ) * argTextureTilingZ));
			terrainMesh.normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}

	ApplyDisplacementMap(argDisplacementMapPath, numOfVertsX, numOfVertsZ);

	/// Creates Diamond Grid Elements by toggling between directions
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

	/// Calculates normals and then creates the terrain geometry
	CalculateNormals();
	CreateGeometry(terrainMesh);

	/// Creates texture for terrain, also applies specular value due to texture not having one
	Helpers::Material mat;
	mat.diffuseTextureFilename = argTextureName;
	mat.specularFactor = 2000;
	CreateTexture(mat);
}

/// Calculates normals using cross product of Triangle edges
void Terrain::CalculateNormals()
{
	/// Updates Vertex Normals
	for (size_t i = 0; i < terrainMesh.elements.size(); i += 3)
	{
		glm::vec3 edge1{ terrainMesh.vertices[terrainMesh.elements[i + 1]] - terrainMesh.vertices[terrainMesh.elements[i]] };
		glm::vec3 edge2{ terrainMesh.vertices[terrainMesh.elements[i + 2]] - terrainMesh.vertices[terrainMesh.elements[i]] };

		glm::vec3 normal{ glm::normalize(glm::cross(edge1, edge2)) };

		terrainMesh.normals[terrainMesh.elements[i]] += normal;
		terrainMesh.normals[terrainMesh.elements[i + 1]] += normal;
		terrainMesh.normals[terrainMesh.elements[i + 2]] += normal;
	}
}

void Terrain::ApplyDisplacementMap(const std::string& argDisplacementMapPath, const int argNumOfVertsX, const int argNumOfVertsZ)
{
	if (argDisplacementMapPath == std::string())
	{
		std::cerr << "ERROR: No displacement map name given" << std::endl;
		return;
	}

	/// Loads displacement map, if fails returns and outputs error
	Helpers::ImageLoader texture;
	if (!texture.Load(argDisplacementMapPath))
	{
		std::cerr << "ERROR: Couldn't find displacement map, path=" << argDisplacementMapPath << std::endl;
		return;
	}

	GLubyte* heightData{ NULL };

	heightData = (GLubyte*)texture.GetData();

	float vertXScale{ static_cast<float>(texture.Width()) / argNumOfVertsX};
	float vertZScale{ static_cast<float>(texture.Height()) / argNumOfVertsZ };

	size_t currentVert = 0;

	/// Adds displacement map height to current height
	for (int z = 0; z < argNumOfVertsZ; z++)
	{
		for (int x = 0; x < argNumOfVertsX; x++)
		{
			float imageX{ vertXScale * x };
			float imageZ{ vertZScale * z };

			size_t offset{ (static_cast<size_t>(imageX) + static_cast<size_t>(imageZ) * texture.Width()) * 4 };

			terrainMesh.vertices[currentVert].y += heightData[offset];

			currentVert += 1;
		}
	}

	/// Recalculates normals after applying new heights
	CalculateNormals();
}
