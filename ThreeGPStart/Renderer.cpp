#include "Renderer.h"
#include "ImageLoader.h"

#include "Terrain.h"

#include "Light.h"
#include "Skybox.h"


// On exit must clean up any OpenGL resources e.g. the program, the buffers
Renderer::~Renderer()
{
	glDeleteProgram(m_program);
//	glDeleteBuffers(1, &m_VAO);
}


// Load, compile and link the shaders and create a program object to host them
bool Renderer::CreateProgram()
{
	// Create a new program (returns a unqiue id)
	m_program = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/vertex_shader.glsl") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/fragment_shader.glsl") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_program, vertex_shader);

	// The attibute 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(m_program, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_program))
		return false;

	return !Helpers::CheckForGLError();
}

void Renderer::CreateMesh(MeshLoadData& argLoadData, std::vector<Renderable*>& argMeshLocation)
{
	argLoadData.isCreated = true;

	switch (argLoadData.meshType)
	{
	case EMeshType::eSkybox:
			
		argMeshLocation.push_back(new Skybox(argLoadData.relativeTransform));
		break;

	case EMeshType::eModel:
		argMeshLocation.push_back(new Model(argLoadData.relativeTransform));
		break;

	default:
		break;
	}

	Model* model{ static_cast<Model*>(argMeshLocation.back()) };
	model->LoadMesh(argLoadData.meshPath);

	for (int i = 0; i < argLoadData.childrenIDs.size(); i++)
	{
		CreateMesh(modelInfomation[argLoadData.childrenIDs[i]], model->children);
	}

}

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry()
{
	// Load and compile shaders into m_program
	if (!CreateProgram())
		return false;

	std::vector<Light> lightData
	{
		/// Type, Position, Direction, FOV, Colour, Range, Intensity
		//Light(ELightType::ePoint, Transform(glm::vec3(0, 100, 0), glm::vec3(0, 0, 0)), 0.0f, glm::vec3(0, 0, 1), 200, 10.0f),
		//Light(ELightType::eSpot, Transform(glm::vec3(0, 10, 0), glm::vec3(0, -1, 0)), 5.0f, glm::vec3(1, 0, 0), 350, 10.0),
		Light(ELightType::eDirectional, Transform(glm::vec3(0, 500, 0), glm::vec3(1, -1, 1)), 0.0f, glm::vec3(1, 1, 1), 0, 1.80f)
	};

	for (const Light& light : lightData)
	{
		Light* newLight = new Light();
		*newLight = light;
		meshes.push_back(newLight);
		Light::numOfLights++;
	}




	/// Loads Models
	for (MeshLoadData& data : modelInfomation)
	{
		if (!data.isCreated)
		{
			CreateMesh(data, meshes);
		}
	}

	// "Data\\3gp_heightmap.bmp"
	// "Data\\curvy.gif"
	std::vector<Terrain> terrainInformation
	{
		Terrain(256, 256, 3000, 3000, 50, 50, "Data\\Models\\Grass.jpg", "Data\\curvy.gif", glm::mat4(1,0,0,0,/**/0,1,0,0,/**/0,0,1,0,/**/0,-100.0f,0,1))
	};

	/// Loads Terrains
	/*for (Terrain& terrain : terrainInformation)
	{
		meshes.push_back(newTerrain);
	}*/

	return true;
}

// Render the scene. Passed the delta time since last called.
void Renderer::Render(const Helpers::Camera& camera, float deltaTime)
{
	// Configure pipeline settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Uncomment to render in wireframe (can be useful when debugging)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Clear buffers from previous frame
	//glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/// Create camera ID and then Sends camera forward direction data to shader as Uniform
	GLuint camera_direcion_id = glGetUniformLocation(m_program, "camera_direction");
	glUniform3fv(camera_direcion_id, 1, glm::value_ptr(camera.GetLookVector()));

	/// Creates and Gets Viewport size
	GLint viewportDimensions[4];
	glGetIntegerv(GL_VIEWPORT, viewportDimensions);

	/// Creates viewport Projection Matrix
	float aspectRatio = viewportDimensions[2] / static_cast<float>(viewportDimensions[3]);
	float nearPlane{ 1.0f }, farPlane{ 12000.0f };
	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspectRatio, nearPlane, farPlane);

	Light::SendNumOfLights(m_program);

	static float angle = 0;
	/// Binds and Draws VAO
	for (const Renderable* renderable : meshes)
	{
		renderable->Draw(m_program, camera, projection_xform, glm::mat4(1));
	}

	// Always a good idea, when debugging at least, to check for GL errors
	Helpers::CheckForGLError();
}
