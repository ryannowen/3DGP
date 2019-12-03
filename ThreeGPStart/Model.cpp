#include "Model.h"

#include "ImageLoader.h"

Model::Model(const Transform& argTransform, const bool argHasLighting)
	: Renderable(argTransform), hasLighting(argHasLighting)
{
}

void Model::Draw(GLuint argProgram, const Helpers::Camera& argCamera, Transform argParentTransform) const
{
	/// Creates and Gets Viewport size
	GLint viewportDimensions[4];
	glGetIntegerv(GL_VIEWPORT, viewportDimensions);

	/// Creates viewport Projection Matrix
	float aspectRatio = viewportDimensions[2] / static_cast<float>(viewportDimensions[3]);
	float nearPlane{ 1.0f }, farPlane{ 12000.0f };
	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspectRatio, nearPlane, farPlane);

	/// Creates View Matrix for camera
	glm::mat4 view_xform = glm::lookAt(argCamera.GetPosition(), argCamera.GetPosition() + argCamera.GetLookVector(), argCamera.GetUpVector());
	glm::mat4 combined_xform = projection_xform * view_xform;


	/// Uses Shaders from our program
	glUseProgram(argProgram);


	/// Create combined xform ID and then Sends Combined Xform data to shader as Uniform
	GLuint combined_xform_id = glGetUniformLocation(argProgram, "combined_xform");
	glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

	/// Create model xform ID for when sending model data
	GLuint model_xform_id = glGetUniformLocation(argProgram, "model_xform");


	for (const SModelData& mesh : subMeshes)
	{
		glm::mat4 model_xform = glm::mat4(1);

		/*Transform newTransform = currentTransform + argParentTransform;


		model_xform = glm::translate(model_xform, newTransform.GetPosition());

		model_xform = glm::rotate(model_xform, newTransform.GetRotation().x, glm::vec3(1, 0, 0));
		model_xform = glm::rotate(model_xform, newTransform.GetRotation().y, glm::vec3(0, 1, 0));
		model_xform = glm::rotate(model_xform, newTransform.GetRotation().z, glm::vec3(0, 0, 1));

		model_xform = glm::scale(model_xform, newTransform.GetScale());*/


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.material.textureRef);

		/// Sends model transformation info as uniform to shader
		glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

		/// Sends Texture Sample to shader
		GLuint texSample_id = glGetUniformLocation(argProgram, "texSample");
		glUniform1i(texSample_id, 0);

		/// Binds and Draws mesh VAO
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.meshElements, GL_UNSIGNED_INT, (void*)0);
	}

	for (const Renderable* renderable : children)
	{
		renderable->Draw(argProgram, argCamera, currentTransform + argParentTransform);
	}
}

void Model::LoadMesh(const std::string& argModelPath)
{
	Helpers::ModelLoader meshes;

	if (!meshes.LoadFromFile(argModelPath))
	{
		std::cerr << "ERROR: Couldn't load Model from file path, path = " << argModelPath << std::endl;
		return;
	}

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
	subMeshes.back().material = newMat;


}
