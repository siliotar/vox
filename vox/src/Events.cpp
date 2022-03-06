#include "Events.hpp"

static uint32_t _keys[1032];
float Events::_deltaX = 0.0f;
float Events::_deltaY = 0.0f;
float Events::_x = 0.0f;
float Events::_y = 0.0f;
bool Events::_cursor_locked = true;
bool Events::_cursor_started = false;
uint32_t Events::_frame = 1;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key > 1031)
		return;
	if (action == GLFW_PRESS)
		_keys[key] = Events::_frame;
	else if (action == GLFW_RELEASE)
		_keys[key] = 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	button += 1024;
	if (button > 1031)
		return;
	if (action == GLFW_PRESS)
		_keys[button] = Events::_frame;
	else if (action == GLFW_RELEASE)
		_keys[button] = 0;
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (Events::_cursor_started)
	{
		Events::_deltaX += float(xpos) - Events::_x;
		Events::_deltaY += float(ypos) - Events::_y;
	}
	else
		Events::_cursor_started = true;
	Events::_x = float(xpos);
	Events::_y = float(ypos);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
}

void Events::init()
{
	glfwSetKeyCallback(Window::window, key_callback);
	glfwSetMouseButtonCallback(Window::window, mouse_button_callback);
	glfwSetCursorPosCallback(Window::window, cursor_pos_callback);
	glfwSetWindowSizeCallback(Window::window, window_size_callback);
}

void Events::poll()
{
	++_frame;
	_deltaX = 0.0f;
	_deltaY = 0.0f;
	glfwPollEvents();
}

bool Events::pressed(uint key)
{
	if (key > 1031)
		return false;
	return _keys[key] == _frame;
}

bool Events::repeat(uint key)
{
	if (key > 1031 || _keys[key] == 0)
		return false;
	return _keys[key] != _frame;
}

bool Events::clicked(uint button)
{
	return pressed(button + 1024);
}

bool Events::clickRepeat(uint button)
{
	return repeat(button + 1024);
}
