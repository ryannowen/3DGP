#include "Skybox.h"

void Skybox::Draw(GLuint argProgram, const Helpers::Camera& argCamera, Transform argParentTransform) const
{
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	/// Creates and Gets Viewport size
	GLint viewportDimensions[4];
	glGetIntegerv(GL_VIEWPORT, viewportDimensions);

	/// Creates viewport Projection Matrix
	float aspectRatio = viewportDimensions[2] / static_cast<float>(viewportDimensions[3]);
	float nearPlane{ 1.0f }, farPlane{ 12000.0f };
	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspectRatio, nearPlane, farPlane);

	/// Creates View Matrix for camera
	glm::mat4 view_xform = glm::lookAt(argCamera.GetPosition(), argCamera.GetPosition() + argCamera.GetLookVector(), argCamera.GetUpVector());
	view_xform = glm::mat4(glm::mat3(view_xform));
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

		Transform newTransform = currentTransform + argParentTransform;


		model_xform = glm::translate(model_xform, newTransform.GetPosition());

		model_xform = glm::rotate(model_xform, newTransform.GetRotation().x, glm::vec3(1, 0, 0));
		model_xform = glm::rotate(model_xform, newTransform.GetRotation().y, glm::vec3(0, 1, 0));
		model_xform = glm::rotate(model_xform, newTransform.GetRotation().z, glm::vec3(0, 0, 1));

		model_xform = glm::scale(model_xform, newTransform.GetScale());


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

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	for (const Renderable* renderable : children)
	{
		renderable->Draw(argProgram, argCamera, currentTransform + argParentTransform);
	}
}
