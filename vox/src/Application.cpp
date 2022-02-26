#include <iostream>
#include "Renderer.hpp"
#include "Events.hpp"
#include "Camera.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
	Window::init(1366, 768, "vox");
	Events::init();
	Camera::init();
	Renderer::init();

	Camera::rotate(glm::radians(89.0f), 0.0f, 0.0f);

	float	deltaTime = 0.0f;
	float	lastTime = glfwGetTime();
	float	movementSpeed = 5.0f;

	float	camX = 0.0f;
	float	camY = 0.0f;

	/* Loop until the user closes the window */
	while (!Window::shouldClose())
	{
		float	currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		Events::poll();

		glm::vec3	move(0.0f);

		if (Events::pressed(GLFW_KEY_W) || Events::repeat(GLFW_KEY_W))
			move.x += 1.0f;
		if (Events::pressed(GLFW_KEY_S) || Events::repeat(GLFW_KEY_S))
			move.x -= 1.0f;
		if (Events::pressed(GLFW_KEY_A) || Events::repeat(GLFW_KEY_A))
			move.y -= 1.0f;
		if (Events::pressed(GLFW_KEY_D) || Events::repeat(GLFW_KEY_D))
			move.y += 1.0f;
		if (Events::pressed(GLFW_KEY_SPACE) || Events::repeat(GLFW_KEY_SPACE))
			move.z += 1.0f;
		if (Events::pressed(GLFW_KEY_LEFT_SHIFT) || Events::repeat(GLFW_KEY_LEFT_SHIFT) || \
			Events::pressed(GLFW_KEY_RIGHT_SHIFT) || Events::repeat(GLFW_KEY_RIGHT_SHIFT))
			move.z -= 1.0f;
		if (move.x != 0.0f || move.y != 0.0f || move.z != 0.0f)
		{
			move = glm::normalize(move);
			Camera::moveFront(move.x * movementSpeed * deltaTime);
			Camera::moveRight(move.y * movementSpeed * deltaTime);
			Camera::moveUp(move.z * movementSpeed * deltaTime);
		}

		if (Events::_cursor_locked)
		{
			camY += Events::_deltaY / Window::height * 2;
			camX -= Events::_deltaX / Window::height * 2;

			if (camY < -glm::radians(89.0f))
				camY = -glm::radians(89.0f);
			if (camY > glm::radians(89.0f))
				camY = glm::radians(89.0f);

			Camera::resetRotation();
			Camera::rotate(camY, camX, 0.0f);
		}

		Renderer::beginBatch();

		Renderer::drawRectangle({ -0.5f, -0.5f,  0.5f }, {  0.5f, -0.5f,  0.5f }, {  0.5f,  0.5f,  0.5f }, { -0.5f,  0.5f,  0.5f }, 0); // Front
		Renderer::drawRectangle({  0.5f, -0.5f,  0.5f }, {  0.5f, -0.5f, -0.5f }, {  0.5f,  0.5f, -0.5f }, {  0.5f,  0.5f,  0.5f }, 0); // Right
		Renderer::drawRectangle({  0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f, -0.5f }, { -0.5f,  0.5f, -0.5f }, {  0.5f,  0.5f, -0.5f }, 0); // Back
		Renderer::drawRectangle({ -0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f,  0.5f }, { -0.5f,  0.5f,  0.5f }, { -0.5f,  0.5f, -0.5f }, 0); // Left
		Renderer::drawRectangle({ -0.5f, -0.5f, -0.5f }, {  0.5f, -0.5f, -0.5f }, {  0.5f, -0.5f,  0.5f }, { -0.5f, -0.5f,  0.5f }, 0); // Down
		Renderer::drawRectangle({ -0.5f,  0.5f,  0.5f }, {  0.5f,  0.5f,  0.5f }, {  0.5f,  0.5f, -0.5f }, { -0.5f,  0.5f, -0.5f }, 1); // Up

		Renderer::endBatch();
		Renderer::flush();

		if (Events::pressed(GLFW_KEY_ESCAPE))
			Window::shouldClose(true);

		Window::swapBuffers();
	}

	Renderer::shutdown();
	Camera::shutdown();
	Window::terminate();

	return EXIT_SUCCESS;
}