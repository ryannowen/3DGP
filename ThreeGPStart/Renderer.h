#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"

#include "ImageLoader.h"

#include <unordered_map>

class Terrain;

struct MeshLoadData
{
	MeshLoadData(const std::string& argMeshPath, const std::vector<std::string>& argTexturePaths, const int argMeshParentID = -1, const glm::mat4 argRelativeTransform = glm::mat4(1))
		: meshPath(argMeshPath), texturePath(argTexturePaths), meshParentID(argMeshParentID), relativeTransform(argRelativeTransform)
	{}

	std::string meshPath;
	std::vector<std::string> texturePath;
	int meshParentID;
	glm::mat4 relativeTransform;
};


struct MeshData
{
	MeshData(GLuint argVAO, unsigned int argMeshElements, GLuint argtextureRef = 0)
		: VAO(argVAO), meshElements(argMeshElements), textureRef(argtextureRef)
	{}


	GLuint VAO{ 0 };
	GLuint textureRef{ 0 };
	unsigned int meshElements{ 0 };
};


struct Mesh
{
	Mesh(int argMeshParentID, glm::mat4 argRelativeTransform)
		: meshParentID(argMeshParentID), relativeTransform(argRelativeTransform)
	{}

	std::vector<MeshData> subMeshs;
	int meshParentID;
	glm::mat4 relativeTransform;
};

enum class ELightType
{
	eDirectional,
	ePoint,
	eSpot
};

struct LightData
{
	ELightType light_type;
	glm::vec3 light_position;
	glm::vec3 light_direction;
	float light_fov;
	glm::vec3 light_colour;
	float light_range;
	float light_intensity;
};

class Renderer
{
private:
	std::vector<Mesh> meshes;
	std::unordered_map<std::string, Helpers::ImageLoader> textureMap;


	// Program object - to host shaders
	GLuint m_program{ 0 };

	// Vertex Array Object to wrap all render settings
	GLuint m_VAO{ 0 };

	// Number of elments to use when rendering
	GLuint m_numElements{ 0 };

	bool CreateProgram();
public:
	Renderer()=default;
	~Renderer();

	std::vector<LightData> lights;

	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();
	void CreateGeometry(const Helpers::Mesh& argMesh);
	void CreateTexture(const Helpers::ImageLoader& argTexture);
	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

