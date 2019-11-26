#include "Model.h"

//void Model::CreateGeometry()
//{
//	/// Reference to Objects
//	GLuint verticesVBO, uvVBO, normalsVBO, elementsEBO;
//
//	/// Generate objects and Bind them with data
//	glGenBuffers(1, &verticesVBO);
//	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * argMesh.vertices.size(), argMesh.vertices.data(), GL_STATIC_DRAW);
//
//	glGenBuffers(1, &normalsVBO);
//	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * argMesh.normals.size(), argMesh.normals.data(), GL_STATIC_DRAW);
//
//	glGenBuffers(1, &uvVBO);
//	glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * argMesh.uvCoords.size(), argMesh.uvCoords.data(), GL_STATIC_DRAW);
//
//	/// Generates Element Buffer and Binds it
//	glGenBuffers(1, &elementsEBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * argMesh.elements.size(), argMesh.elements.data(), GL_STATIC_DRAW);
//
//	/// Clear bindings
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	/// Generates VAO
//	glGenVertexArrays(1, &m_VAO);
//	glBindVertexArray(m_VAO);
//
//	/// Sends Vertex Positions
//	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//
//	/// Sends Normals
//	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//
//	/// Sends uvs
//	glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
//
//
//
//	/// Binds Elements
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);
//
//
//	MeshData newMeshData(m_VAO, argMesh.elements.size());
//	meshes.back().subMeshs.push_back(newMeshData);
//
//	// Good idea to check for an error now:	
//	Helpers::CheckForGLError();
//
//	// Clear VAO binding
//	glBindVertexArray(0);
//}
//
//void Model::CreateTexture()
//{
//}
