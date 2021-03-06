#include "Simulation.h"
#include "Renderer.h"

// Initialise this as well as the renderer, returns false on error
bool Simulation::Initialise()
{
	srand(0);

	// Set up camera
	m_camera = std::make_shared<Helpers::Camera>();
	m_camera->Initialise(glm::vec3(-749.5, 1034.1, 1223.6), glm::vec3(0.5531, 0.52512, 0));


	std::cout << "Jeep Controls" << std::endl;
	std::cout << "Num8, Num4, Num5, Num6 - moves the jeep left, right (x axis) and into and out of the scene (Z axis)" << std::endl;
	std::cout << "Num7 and Num9 - moves the jeep up and down the Y axis.\n\n" << std::endl;

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

	if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) // Right
		pos1.x = 10;
	else if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) // Left
		pos1.x = -10;
	if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS) // Up
		pos1.y = -10;
	else if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS) // Down
		pos1.y = 10;
	if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) // Forward
		pos1.z = -10;
	else if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) // Back
		pos1.z = 10;

	/// Jeep
	Model* jeep_spin = static_cast<Model*>(m_renderer->FindRenderable("Jeep_Spin"));
	if (jeep_spin != nullptr)
	{
		jeep_spin->currentTransform.AddPosition(pos1);
		jeep_spin->currentTransform.AddRotation(glm::vec3(0, 2, 0));
	}

	/// Point Light movement
	Light* point = static_cast<Light*>(m_renderer->FindRenderable("Light_Point"));
	if (point != nullptr)
	{
		point->currentTransform.AddPosition(sin(glm::vec3(5,0,0) * m_lastTime) * 20.0f);
	}

	/// Aquapig Hull
	Model* AquaPig{ static_cast<Model*>(m_renderer->FindRenderable("Aqua_Hull")) };
	if (AquaPig != nullptr)
	{
		/// Aquapig Prop
		Renderable* propeller{ AquaPig->FindChild("Aqua_Propeller") };
		if (propeller != nullptr)
			static_cast<Model*>(propeller)->currentTransform.AddRotation(glm::vec3(0, 10, 0));

		/// Aquapig Gunbase
		Renderable* gunbase{ AquaPig->FindChild("Aqua_Gunbase") };
		if (gunbase != nullptr)
		{
			static_cast<Model*>(gunbase)->currentTransform.AddRotation(glm::vec3(0, sin(2 * m_lastTime) + 0.5, 0));

			/// Aquapig Gun
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
