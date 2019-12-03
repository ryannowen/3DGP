#include "Renderer.h"
#include "ImageLoader.h"

#include "Terrain.h"


// On exit must clean up any OpenGL resources e.g. the program, the buffers
Renderer::~Renderer()
{
	glDeleteProgram(m_program);
	glDeleteBuffers(1, &m_VAO);
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

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry()
{
	// Load and compile shaders into m_program
	if (!CreateProgram())
		return false;

	MeshLoadData modelInfomation[]
	{
		MeshLoadData("Data\\Sky\\Hills\\skybox.x", std::vector <std::string>{"Data\\Sky\\Hills\\skybox_top.JPG", "Data\\Sky\\Hills\\skybox_right.JPG", "Data\\Sky\\Hills\\skybox_left.JPG", "Data\\Sky\\Hills\\skybox_front.JPG", "Data\\Sky\\Hills\\skybox_back.JPG", "Data\\Sky\\Hills\\skybox_bottom.JPG",}, -1, glm::mat4(1,0,0,0,/**/0,1,0,0,/**/0,0,1,0,/**/0,0,0,0.003f)),
		MeshLoadData("Data\\Models\\AquaPig\\hull.obj", std::vector<std::string>{"Data\\Models\\AquaPig\\aqua_pig_2K.png"}, -1, glm::mat4(1,0,0,0,/**/0,1,0,0,/**/0,0,1,0,/**/150,0,0,30)),
			MeshLoadData("Data\\Models\\AquaPig\\wing_right.obj", std::vector <std::string>{"Data\\Models\\AquaPig\\aqua_pig_2K.png"}, 1, glm::mat4(1,0,0,0,/**/0,1,0,0,/**/0,0,1,0,/**/-2.231,0.272,-2.663,1)),
			MeshLoadData("Data\\Models\\AquaPig\\wing_left.obj", std::vector <std::string>{"Data\\Models\\AquaPig\\aqua_pig_2K.png"}, 1, glm::mat4(1,0,0,0,/**/0,1,0,0,/**/0,0,1,0,/**/2.231,0.272,-2.663,1)),
			MeshLoadData("Data\\Models\\AquaPig\\propeller.obj", std::vector <std::string>{"Data\\Models\\AquaPig\\aqua_pig_2K.png"}, 1, glm::mat4(1,0,0,0,/**/0,0,1,0,/**/0,-1,0,0,/**/0,1.395,-3.616,1)),
			MeshLoadData("Data\\Models\\AquaPig\\gun_base.obj", std::vector <std::string>{"Data\\Models\\AquaPig\\aqua_pig_2K.png"}, 1, glm::mat4(1,0,0,0,/**/0,1,0,0,/**/0,0,1,0,/**/0,0.569,-1.866,1)),
				MeshLoadData("Data\\Models\\AquaPig\\gun.obj", std::vector <std::string>{"Data\\Models\\AquaPig\\aqua_pig_2K.png"}, 5, glm::mat4(1,0,0,0,/**/0,1,0,0,/**/0,0,1,0,/**/0,1.506,0.644,1)),
		MeshLoadData("Data\\Models\\Jeep\\jeep.obj", std::vector <std::string>{"Data\\Models\\Jeep\\jeep_army.jpg"}, -1, glm::mat4(0,0,1,0,/**/0,1,0,0,/**/-1,0,0,0,/**/-150,0,0,0.25))
	};

	// "Data\\3gp_heightmap.bmp"
	// "Data\\curvy.gif"
	Terrain terrainInformation[]
	{
		Terrain(512, 512, 3000, 3000, 50, 50, "Data\\Models\\Grass.jpg", "Data\\curvy.gif", glm::mat4(1,0,0,0,/**/0,1,0,0,/**/0,0,1,0,/**/0,-100.0f,0,1))
	};

	/// Loads Models
	Helpers::ModelLoader modelloader;
	for (const MeshLoadData& MeshInfo : modelInfomation)
	{

		if (!modelloader.LoadFromFile(MeshInfo.meshPath))
			return false;

		meshes.push_back(Mesh(MeshInfo.meshParentID, MeshInfo.relativeTransform));

		const std::vector<Helpers::Mesh>& models = modelloader.GetMeshVector();


		for (int i = 0, currentTexture = 0; i < models.size(); i++, currentTexture++)
		{
			/// Loads Texture from file
			Helpers::ImageLoader texture;
			if (currentTexture >= MeshInfo.texturePath.size())
			{
				std::cerr << models.size() << std::endl;
				std::cerr << "Mesh Textures weren't specified" << std::endl;
				//currentTexture = MeshInfo.texturePath.size() - 1;
				return false;
			}

			/// Checks if texture is loaded, if it isn't then load it and save into texture map, otherwise use already loaded
			if (textureMap.find(MeshInfo.texturePath[currentTexture]) == textureMap.end())
			{
				if (!texture.Load(MeshInfo.texturePath[currentTexture]))
					return false;

				textureMap[MeshInfo.texturePath[currentTexture]] = texture;
			}
			else
			{
				texture = textureMap.at(MeshInfo.texturePath[currentTexture]);
			}

			CreateGeometry(models[i]);
			CreateTexture(texture);
		}

		modelloader.GetMeshVector().erase(modelloader.GetMeshVector().begin(), modelloader.GetMeshVector().end());
	}


	/// Loads Terrains
	for (Terrain& terrain : terrainInformation)
	{
		meshes.push_back(Mesh(-1, terrain.xform));

		CreateGeometry(terrain.terrainMesh);

		if (terrain.texturePath != std::string())
		{
			Helpers::ImageLoader texture;
			if (textureMap.find(terrain.texturePath) == textureMap.end())
			{
				if (!texture.Load(terrain.texturePath))
					return false;

				textureMap[terrain.texturePath] = texture;
			}
			else
			{
				texture = textureMap.at(terrain.texturePath);
			}

			CreateTexture(texture);
		}
	}


	std::vector<LightData> lightData
	{
		/// Type, Position, Direction, FOV, Colour, Range, Intensity
		{ELightType::ePoint, glm::vec3(0, 100, 0), glm::vec3(0, 0, 0), 0.0f, glm::vec3(0, 0, 1), 200, 10.0f},
		{ELightType::eSpot, glm::vec3(-100, 100, 0), glm::vec3(0, -1, 0), 5.0f, glm::vec3(1, 0, 0), 350, 10.0f},
		{ELightType::eDirectional, glm::vec3(0, 500, 0), glm::vec3(1, -1, 0), 0.0f, glm::vec3(1, 1, 1), 0, 0.80f},
	};

	lights = lightData;
	
	return true;
}


void Renderer::CreateGeometry(const Helpers::Mesh& argMesh)
{
	/// Reference to Objects
	GLuint verticesVBO, uvVBO, normalsVBO, elementsEBO;

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


	MeshData newMeshData(m_VAO, argMesh.elements.size());
	meshes.back().subMeshs.push_back(newMeshData);

	// Good idea to check for an error now:	
	Helpers::CheckForGLError();

	// Clear VAO binding
	glBindVertexArray(0);
}

void Renderer::CreateTexture(const Helpers::ImageLoader& argTexture)
{
	GLuint textureRef;

	/// Generates texture
	glGenTextures(1, &textureRef);
	glBindTexture(GL_TEXTURE_2D, textureRef);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, argTexture.Width(), argTexture.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, argTexture.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);


	meshes.back().subMeshs.back().textureRef = textureRef;
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

	/// Creates and Gets Viewport size
	GLint viewportDimensions[4];
	glGetIntegerv(GL_VIEWPORT, viewportDimensions);

	/// Creates viewport Projection Matrix
	float aspectRatio = viewportDimensions[2] / static_cast<float>(viewportDimensions[3]);
	float nearPlane{ 1.0f }, farPlane{ 12000.0f };
	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspectRatio, nearPlane, farPlane);

	/// Creates View Matrix for camera
	glm::mat4 view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());
	glm::mat4 combined_xform;

	/// Uses Shaders from our program
	glUseProgram(m_program);

	/// Create combined xform ID and then Sends Combined Xform data to shader as Uniform
	GLuint combined_xform_id = glGetUniformLocation(m_program, "combined_xform");

	/// Create model xform ID for when sending model data
	GLuint model_xform_id = glGetUniformLocation(m_program, "model_xform");

	/// Create camera ID and then Sends camera forward direction data to shader as Uniform
	GLuint camera_direcion_id = glGetUniformLocation(m_program, "camera_direction");
	glUniform3fv(camera_direcion_id, 1, glm::value_ptr(camera.GetLookVector()));

	/// Create  ID and then Sends  data to shader as Uniform
	GLuint numOfLights_id = glGetUniformLocation(m_program, "numOfLights");
	glUniform1i(numOfLights_id, static_cast<int>(lights.size()));


	/// Create light IDs and then Sends each light data to shader array as Uniform
	for (int i = 0; i < lights.size(); i++)
	{
		GLuint light_type_id = glGetUniformLocation(m_program, std::string("lights[" + std::to_string(i) + "].light_type").c_str());
		glUniform1i(light_type_id, static_cast<int>(lights.at(i).light_type));

		GLuint light_position_id = glGetUniformLocation(m_program, std::string("lights[" + std::to_string(i) + "].light_position").c_str());
		glUniform3fv(light_position_id, 1, glm::value_ptr(lights.at(i).light_position));

		GLuint light_direction_id = glGetUniformLocation(m_program, std::string("lights[" + std::to_string(i) + "].light_direction").c_str());
		glUniform3fv(light_direction_id, 1, glm::value_ptr(lights.at(i).light_direction));

		GLuint light_fov_id = glGetUniformLocation(m_program, std::string("lights[" + std::to_string(i) + "].light_fov").c_str());
		glUniform1f(light_fov_id, lights.at(i).light_fov);

		GLuint light_colour_id = glGetUniformLocation(m_program, std::string("lights[" + std::to_string(i) + "].light_colour").c_str());
		glUniform3fv(light_colour_id, 1, glm::value_ptr(lights.at(i).light_colour));

		GLuint light_range_id = glGetUniformLocation(m_program, std::string("lights[" + std::to_string(i) + "].light_range").c_str());
		glUniform1f(light_range_id, lights.at(i).light_range);

		GLuint light_intensity_id = glGetUniformLocation(m_program, std::string("lights[" + std::to_string(i) + "].light_intensity").c_str());
		glUniform1f(light_intensity_id, lights.at(i).light_intensity);
	}

	static float angle = 0;
	/// Binds and Draws VAO
	for (int i = 0; i < meshes.size(); i++)
	{
		glm::mat4 model_xform = glm::mat4(1);

		for (const MeshData& subMesh : meshes[i].subMeshs)
		{
			/// Creates Identity Matrix for base transformation and then rotates it to create spin effect
	
			//model_xform = glm::rotate(model_xform, angle, glm::vec3{ 0, -1, 0 });

			/// applys transformation to initial mesh
			glm::vec3 translation;
			translation.x = meshes[i].relativeTransform[3][0];
			translation.y = meshes[i].relativeTransform[3][1];
			translation.z = meshes[i].relativeTransform[3][2];
			float scale = meshes[i].relativeTransform[3][3];

			model_xform = glm::translate(model_xform, translation);
			model_xform = glm::scale(model_xform, glm::vec3(scale));


			//model_xform = meshes[i].relativeTransform;

			/// Gets current parent to work on
			int currentMeshParentID{ meshes[i].meshParentID };

			/// Cycles through meshes till parent is found, calculating the models local transform
			while (currentMeshParentID != -1)
			{
				translation.x = meshes[currentMeshParentID].relativeTransform[3][0];
				translation.y = meshes[currentMeshParentID].relativeTransform[3][1];
				translation.z = meshes[currentMeshParentID].relativeTransform[3][2];
				float scale = meshes[currentMeshParentID].relativeTransform[3][3];

				model_xform = glm::translate(model_xform, translation);
				model_xform = glm::scale(model_xform, glm::vec3(scale));
				//model_xform = meshes[currentMeshParentID].relativeTransform * model_xform;
				currentMeshParentID = meshes[currentMeshParentID].meshParentID;
			}

			/// Sends model transformation info as uniform to shader
			glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

			if (i == 0) /// Checks if is first model (For skybox)
			{
				glm::mat4 view_xform2 = glm::mat4(glm::mat3(view_xform));
				combined_xform = projection_xform * view_xform2;

				glDepthMask(GL_FALSE);
				glDisable(GL_DEPTH_TEST);
			}
			else
			{
				combined_xform = projection_xform * view_xform;
				glDepthMask(GL_TRUE);
				glEnable(GL_DEPTH_TEST);
			}

			glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, subMesh.textureRef);

			/// Sends Texture Sample to shader
			GLuint texSample_id = glGetUniformLocation(m_program, "texSample");
			glUniform1i(texSample_id, 0);

			/// Binds and Draws mesh VAO
			glBindVertexArray(subMesh.VAO);
			glDrawElements(GL_TRIANGLES, subMesh.meshElements, GL_UNSIGNED_INT, (void*)0);
		}

		/// Adds to rotation
		angle += 0.01f;


		/// Rotates Propellor Mesh
	//	if (meshes.size() >= 4)
		//	meshes[4].relativeTransform = glm::rotate(meshes[4].relativeTransform, 0.2f, glm::vec3{ 0, 1, 0 });
	}

	// Always a good idea, when debugging at least, to check for GL errors
	Helpers::CheckForGLError();
}
