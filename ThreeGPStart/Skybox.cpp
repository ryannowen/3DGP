#include "Skybox.h"


Skybox::Skybox(const Transform& argTransform, const bool argHasLighting)
	:	Model(argTransform, argHasLighting)
{

}

void Skybox::Draw(GLuint argProgram, const Helpers::Camera& argCamera, const glm::mat4& argProjection_Xform, glm::mat4 argParentTransform) const
{
	/// Creates View Matrix for camera
	glm::mat4 view_xform = glm::lookAt(argCamera.GetPosition(), argCamera.GetPosition() + argCamera.GetLookVector(), argCamera.GetUpVector());
	view_xform = glm::mat4(glm::mat3(view_xform));
	glm::mat4 combined_xform = argProjection_Xform * view_xform;

	/// Disables Depth Testing
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	/// Uses Shaders from our program
	glUseProgram(argProgram);

	/// Create hasLighting ID and then Sends bool data to shader as Uniform
	GLuint hasLighting_id = glGetUniformLocation(argProgram, "hasLighting");
	glUniform1i(hasLighting_id, GL_FALSE);

	/// Create combined xform ID and then Sends Combined Xform data to shader as Uniform
	GLuint combined_xform_id = glGetUniformLocation(argProgram, "combined_xform");
	glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

	/// Create model xform ID for when sending model data
	GLuint model_xform_id = glGetUniformLocation(argProgram, "model_xform");


	for (const SModelData& mesh : subMeshes)
	{
		mesh.material.ApplyMaterial(argProgram);

		glm::mat4 model_xform = glm::mat4(1);

		/// Sends model transformation info as uniform to shader
		glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

		/// Binds and Draws mesh VAO
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.meshElements, GL_UNSIGNED_INT, (void*)0);
	}

	/// Enables Depth Testing
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	/// Renders all children
	for (const Renderable* renderable : children)
	{
		renderable->Draw(argProgram, argCamera, argProjection_Xform, argParentTransform);
	}
}
