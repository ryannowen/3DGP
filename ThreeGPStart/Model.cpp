#include "Model.h"

#include "ImageLoader.h"

Model::Model()
{
}

void Model::Draw(GLuint argProgram, Helpers::Camera& argCamera) const
{
}

void Model::LoadMesh(const std::string& argModelPath)
{
	Helpers::ModelLoader meshes;

	if (!meshes.LoadFromFile(argModelPath))
	{
		std::cerr << "ERROR: Couldn't load Model from file path, path = " << argModelPath << std::endl;
		return;
	}

	/*for (Helpers::Mesh mesh : meshes.GetMeshVector())
	{
		CreateGeometry(mesh);
	}

	for (Helpers::Material material : meshes.GetMaterialVector())
	{
		CreateTexture(material);
	}*/

	for (int i = 0; i < meshes.GetMeshVector().size(); i++)
	{
		CreateGeometry(meshes.GetMeshVector()[i]);
		CreateTexture(meshes.GetMaterialVector()[i]);
	}

}

void Model::CreateGeometry(const Helpers::Mesh& argMesh)
{
	/// Reference to Objects
	GLuint m_VAO, verticesVBO, uvVBO, normalsVBO, elementsEBO;

	/// Generate objects and Bind them with data
	glGenBuffers(1, &verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * argMesh.vertices.size(), argMesh.vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * argMesh.normals.size(), argMesh.normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &uvVBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * argMesh.uvCoords.size(), argMesh.uvCoords.data(), GL_STATIC_DRAW);

	/// Generates Element Buffer and Binds it
	glGenBuffers(1, &elementsEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * argMesh.elements.size(), argMesh.elements.data(), GL_STATIC_DRAW);

	/// Clear bindings
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/// Generates VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	/// Sends Vertex Positions
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/// Sends Normals
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/// Sends uvs
	glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);



	/// Binds Elements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);


	SModelData newModelData(m_VAO, argMesh.elements.size());
	subMeshes.push_back(newModelData);

	// Good idea to check for an error now:	
	Helpers::CheckForGLError();

	// Clear VAO binding
	glBindVertexArray(0);
}

void Model::CreateTexture(const Helpers::Material& argMat)
{
	/// Loads Texture from file
	Helpers::ImageLoader texture;
	if (!texture.Load("Data\\Textures\\" + argMat.diffuseTextureFilename))
		return;

	GLuint textureRef;

	/// Generates texture
	glGenTextures(1, &textureRef);
	glBindTexture(GL_TEXTURE_2D, textureRef);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.Width(), texture.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);

	Material newMat(argMat.ambientColour, argMat.specularColour, argMat.specularFactor, textureRef);
	subMeshes.back().materials.push_back(newMat);


}
