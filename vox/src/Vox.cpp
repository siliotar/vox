#include "vox.hpp"
#include "Window.hpp"
#include "Events.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "UI.hpp"
#include "Settings.hpp"
#include "BlocksInfo.hpp"

#include <sstream>
#include <iostream>

void hello(float)
{
	std::cout << "Hello! :)" << std::endl;
}

Vox::Vox(int screenWidth, int screenHeight)
{
	Window::init(screenWidth, screenHeight, "vox");

	FrameBufferSpecification fbSpec;
	fbSpec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
	fbSpec.Width = Window::width;
	fbSpec.Height = Window::height;
	_fb = new FrameBuffer(fbSpec);

	BlocksInfo::init();
	Events::init();
	Camera::init();
	Renderer::init();
	UI::init();
}

Vox::~Vox()
{
	UI::terminate();
	Renderer::shutdown();
	Camera::shutdown();
	Window::terminate();
	delete _fb;
	BlocksInfo::shutdown();
}

void Vox::Run()
{
	double	deltaTime = 0.0f;
	double	lastTime = glfwGetTime();
	float	movementSpeed = MovementSpeed;

	float	camX = 0.0f;
	float	camY = 0.0f;

	Map	map;
	map.setBlock(0, 5, 0, 0);
	map.setBlock(1, 5, 1, 0);
	map.setBlock(2, 5, 2, 0);
	map.setBlock(3, 5, 3, 0);
	map.setBlock(4, 5, 4, 0);
	map.setBlock(5, 0, 5, 2);

	UI::addElement(std::make_shared<Button>(-0.5f, 0.8f, 0.1f, 0.1f, hello), "test");
	UI::addElement(std::make_shared<Button>(0.4f, 0.8f, 0.1f, 0.1f, hello), "test");

	size_t frames = 0;
	double startTime = glfwGetTime();
	double lastFPSTime = startTime;

	glClearColor(0.2f, 0.3f, 0.6f, 0.0f);

	/* Loop until the user closes the window */
	while (!Window::shouldClose())
	{
		double	currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		if (currentTime - lastFPSTime >= 1.0)
		{
			double fps = double(frames) / (currentTime - lastFPSTime);
			std::ostringstream	ss;
			ss << "vox [" << fps << " FPS]";
			Window::setTitle(ss.str());
			lastFPSTime = currentTime;
			frames = 0;
		}

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
		if (Events::pressed(GLFW_KEY_LEFT_CONTROL) || Events::repeat(GLFW_KEY_LEFT_CONTROL) || \
			Events::pressed(GLFW_KEY_RIGHT_CONTROL) || Events::repeat(GLFW_KEY_RIGHT_CONTROL))
			movementSpeed = SprintSpeed;
		else
			movementSpeed = MovementSpeed;
		if (move.x != 0.0f || move.y != 0.0f || move.z != 0.0f)
		{
			move = glm::normalize(move);
			Camera::moveFront(move.x * movementSpeed * float(deltaTime));
			Camera::moveRight(move.y * movementSpeed * float(deltaTime));
			Camera::moveUp(move.z * movementSpeed * float(deltaTime));
		}

		if (Events::pressed(GLFW_KEY_TAB))
		{
			if (Events::_cursor_locked)
				Window::displayCursor();
			else
				Window::hideCursor();
			std::cout << Camera::getPlayerPosition().x << ", " << Camera::getPlayerPosition().y << ", " << Camera::getPlayerPosition().z << std::endl;
		}

		if (Events::_cursor_locked)
		{
			camY += Events::_deltaY / Window::height * 2;
			camX -= Events::_deltaX / Window::width * 2;

			if (camY < -glm::radians(89.0f))
				camY = -glm::radians(89.0f);
			if (camY > glm::radians(89.0f))
				camY = glm::radians(89.0f);

			Camera::resetRotation();
			Camera::rotate(camY, camX, 0.0f);
		}

		if (_fb->getSpecification().Width != Window::width || _fb->getSpecification().Height != Window::height)
			_fb->resize(Window::width, Window::height);

		_fb->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_fb->clearAttachment(1, -1);
		Renderer::drawMap(map);
#ifdef QWE
		UI::draw("test");

		if (!Events::_cursor_locked && Events::clicked(GLFW_MOUSE_BUTTON_LEFT))
		{
			int invertedY = Window::height - Events::_y - 1.0f;
			int32_t id = fb.readPixel(1, Events::_x, invertedY);
			UI::call(id, Events::_x, invertedY);
		}
#endif
		_fb->draw();

		if (Events::pressed(GLFW_KEY_ESCAPE))
			Window::shouldClose(true);

		Window::swapBuffers();
		++frames;
	}
}
