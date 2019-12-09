#include "Simulation.h"
#include "Renderer.h"

// Initialise this as well as the renderer, returns false on error
bool Simulation::Initialise()
{
	// Set up camera
	m_camera = std::make_shared<Helpers::Camera>();
	m_camera->Initialise(glm::vec3(0, 600, 1500), glm::vec3(0)); // Jeep
	//m_camera->Initialise(glm::vec3(-13.82f, 5.0f, 1.886f), glm::vec3(0.25f, 1.5f, 0), 30.0f,0.8f); // Aqua pig

	// Set up renderer
	m_renderer = std::make_shared<Renderer>();
	return m_renderer->InitialiseGeometry();
}

// Handle any user input. Return false if program should close.
bool Simulation::HandleInput(GLFWwindow* window)
{
	// You can get keypresses like this:
	// if (glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS) // means W key pressed

	if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glm::vec3 pos1(0);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // Right
		pos1.x = 10;
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // Left
		pos1.x = -10;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // Up
		pos1.y = -10;
	else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) // Down
		pos1.y = 10;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // Forward
		pos1.z = -10;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // Back
		pos1.z = 10;


	Model* jeep = static_cast<Model*>(m_renderer->FindRenderable("Jeep"));
	if (jeep != nullptr)
	{
		jeep->currentTransform.AddPosition(pos1);
		jeep->currentTransform.AddRotation(glm::vec3(0, 2, 0));
	}

	Light* spot = static_cast<Light*>(m_renderer->FindRenderable("Light_Spot_Right"));
	if (spot != nullptr)
	{
		//spot->currentTransform.AddPosition(pos1);
		//spot->currentTransform.AddRotation(glm::vec3(0, 0.1f, 0));
	}

	Light* point = static_cast<Light*>(m_renderer->FindRenderable("Light_Point"));
	if (point != nullptr)
	{
		point->currentTransform.AddPosition(sin(glm::vec3(5,0,0) * m_lastTime) * 20.0f);
	}

	Model* AquaPig{ static_cast<Model*>(m_renderer->FindRenderable("Aqua_Hull")) };
	if (AquaPig != nullptr)
	{
		Renderable* propeller{ AquaPig->FindChild("Aqua_Propeller") };
		if (propeller != nullptr)
			static_cast<Model*>(propeller)->currentTransform.AddRotation(glm::vec3(0, 10, 0));

		Renderable* gunbase{ AquaPig->FindChild("Aqua_Gunbase") };
		if (gunbase != nullptr)
		{
			static_cast<Model*>(gunbase)->currentTransform.AddRotation(glm::vec3(0, sin(2 * m_lastTime) + 0.5, 0));

			Renderable* gun{ static_cast<Model*>(gunbase)->FindChild("Aqua_Gun") };
			if (gun != nullptr)
				static_cast<Model*>(gun)->currentTransform.AddRotation(glm::vec3(sin(5 * m_lastTime) * -2, 0, 0));
		}
	}

	// int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

	// Use this to get the mouse position:
	// double xpos, ypos;
	// glfwGetCursorPos(window, &xpos, &ypos);

	// To prevent the mouse leaving the window (and to hide the arrow) you can call:
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// To reenable it use GLFW_CURSOR_NORMAL

	return true;
}

// Update the simulation (and render) returns false if program should close
bool Simulation::Update(GLFWwindow* window)
{
	// Deal with any input
	if (!HandleInput(window))
		return false;

	// Calculate delta time since last called
	// We pass the delta time to the camera and renderer
	float timeNow = (float)glfwGetTime();
	float deltaTime{ timeNow - m_lastTime };
	m_lastTime = timeNow;

	// The camera needs updating to handle user input internally
	m_camera->Update(window, deltaTime);

	// Render the scene
	m_renderer->Render(*m_camera, deltaTime);

	return true;
}
