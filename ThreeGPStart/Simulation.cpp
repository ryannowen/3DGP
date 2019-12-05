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

	int x{ 0 }, y{ 0 }, z{ 0 };

	//if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // Right
	//	x = 10;
	//else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // Left
	//	x = -10;
	//if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // Up
	//	y = -10;
	//else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) // Down
	//	y = 10;
	//if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // Forward
	//	z = -10;
	//else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // Back
	//	z = 10;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		static_cast<Model*>(m_renderer->renderables[2])->currentTransform.AddRotation(glm::vec3(0, 90, 0));

	//m_renderer->lights[0].light_position += glm::vec3(x, y, z);

	// int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

	// Use this to get the mouse position:
	// double xpos, ypos;
	// glfwGetCursorPos(window, &xpos, &ypos);

	// To prevent the mouse leaving the window (and to hide the arrow) you can call:
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// To reenable it use GLFW_CURSOR_NORMAL

	// To see an example of input using GLFW see the camera.cpp file.
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
