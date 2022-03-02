#include "Window.hpp"

GLFWwindow* Window::window = nullptr;
int Window::width = 0;
int Window::height = 0;

void	Window::init(int w, int h, const std::string& title)
{
	width = w;
	height = h;
	if (window != nullptr)
		return;
	if (!glfwInit())
	{
		std::cerr << "Failed to init glfw!" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

#ifdef __linux__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(width, height, title.c_str(), 0, 0);
	if (!window)
	{
		std::cerr << "Failed to create window!" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(0);

#ifndef _WIN32
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to init glad!" << std::endl;
#else
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to init glew!" << std::endl;
#endif
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glEnable(GL_DEPTH_TEST);
	hideCursor();
	glEnable(GL_CULL_FACE);
}

bool	Window::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void	Window::swapBuffers()
{
	glfwSwapBuffers(window);
}

void	Window::terminate()
{
	glfwTerminate();
}

void Window::shouldClose(bool flag)
{
	glfwSetWindowShouldClose(window, flag);
}

void	Window::displayCursor()
{
	glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//Events::_cursor_locked = false;
}

void	Window::hideCursor()
{
	glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//Events::_cursor_locked = true;
}

void	Window::setTitle(const std::string& title)
{
	glfwSetWindowTitle(window, title.c_str());
}
