#include "Terrain.h"

#include "PerlinNoise.h"

Terrain::Terrain()
{
}

void Terrain::CreateTerrain(const int argNumCellsX, const int argNumCellsZ, const float argSizeX, const float argSizeZ, const int argTextureTilingX, const int argTextureTilingZ, const std::string& argTextureName, const std::string& argDisplacementMapPath)
{
	xTiling = argTextureTilingX;
	zTiling = argTextureTilingZ;

	int numOfCellsX{ argNumCellsX };
	int numOfCellsZ{ argNumCellsZ };

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
			terrainMesh.vertices.push_back(glm::vec3(worldPositionX + (x * vertexXSpacing), 0, worldPositionZ + (-z * vertexZSpacing)));

			/// Applies initial vert height using perlin noise
			glm::vec3 currentVert{ terrainMesh.vertices.back() };
			terrainMesh.vertices.back().y = PerlinNoise::Perlin(currentVert.x, currentVert.z) * 5;

			terrainMesh.uvCoords.push_back(glm::vec2(x / static_cast<float>(numOfCellsX) * xTiling, z / static_cast<float>(numOfCellsZ) * zTiling));
			terrainMesh.normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	ApplyDisplacementMap(argDisplacementMapPath, numOfVertsX, numOfVertsZ);

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

	CalculateNormals();

	CreateGeometry(terrainMesh);

	Helpers::Material mat;

	mat.diffuseTextureFilename = argTextureName;
	mat.specularFactor = 0;

	CreateTexture(mat);
}

void Terrain::CalculateNormals()
{
	/// Updates Vertex Normals
	for (int i = 0; i < terrainMesh.elements.size(); i += 3)
	{
		glm::vec3 edge1{ terrainMesh.vertices[terrainMesh.elements[i + 1]] - terrainMesh.vertices[terrainMesh.elements[i]] };
		glm::vec3 edge2{ terrainMesh.vertices[terrainMesh.elements[i + 2]] - terrainMesh.vertices[terrainMesh.elements[i]] };

		glm::vec3 normal{ glm::normalize(glm::cross(edge1, edge2)) };

		terrainMesh.normals[terrainMesh.elements[i]] += normal;
		terrainMesh.normals[terrainMesh.elements[i + 1]] += normal;
		terrainMesh.normals[terrainMesh.elements[i + 2]] += normal;
	}

	//for (glm::vec3& normal : terrainMesh.normals)
		//normal = glm::normalize(normal);
}

void Terrain::ApplyDisplacementMap(const std::string& argDisplacementMapPath, const int argNumOfVertsX, const int argNumOfVertsZ)
{
	if (argDisplacementMapPath != std::string())
	{
		GLubyte* heightData{ NULL };

		if (texture.Load(argDisplacementMapPath))
		{
			heightData = (GLubyte*)texture.GetData();

			float vertXScale{ static_cast<float>(texture.Width()) / argNumOfVertsX};
			float vertZScale{ static_cast<float>(texture.Height()) / argNumOfVertsZ };

			size_t currentVert = 0;

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

			CalculateNormals();
		}
	}
}
