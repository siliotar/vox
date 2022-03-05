#pragma once

#include "Window.hpp"

typedef unsigned int uint;

namespace Events
{
	enum class KeyState
	{
		Released,
		Pressed,
		Repeat
	};

	static KeyState _keys[1032];
	static float _deltaX = 0.0f;
	static float _deltaY = 0.0f;
	static float _x = 0.0f;
	static float _y = 0.0f;
	static bool _cursor_locked = true;
	static bool _cursor_started = false;


	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key > 1031)
			return;
		if (action == GLFW_PRESS)
			_keys[key] = KeyState::Pressed;
		else if (action == GLFW_REPEAT)
			_keys[key] = KeyState::Repeat;
		else if (action == GLFW_RELEASE)
			_keys[key] = KeyState::Released;
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
	{
		button += 1024;
		if (button > 1031)
			return;
		if (action == GLFW_PRESS)
			_keys[button] = KeyState::Pressed;
		else if (action == GLFW_REPEAT)
			_keys[button] = KeyState::Repeat;
		else if (action == GLFW_RELEASE)
			_keys[button] = KeyState::Released;
	}

	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (_cursor_started)
		{
			_deltaX += float(xpos) - _x;
			_deltaY += float(ypos) - _y;
		}
		else
			_cursor_started = true;
		_x = float(xpos);
		_y = float(ypos);
	}

	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		//glViewport(0, 0, width, height);
		Window::width = width;
		Window::height = height;
	}

	static void init()
	{
		glfwSetKeyCallback(Window::window, key_callback);
		glfwSetMouseButtonCallback(Window::window, mouse_button_callback);
		glfwSetCursorPosCallback(Window::window, cursor_pos_callback);
		glfwSetWindowSizeCallback(Window::window, window_size_callback);
	}

	static void poll()
	{
		_deltaX = 0.0f;
		_deltaY = 0.0f;
		glfwPollEvents();
	}

	static bool pressed(uint key)
	{
		if (key > 1031)
			return false;
		return _keys[key] == KeyState::Pressed;
	}

	static bool repeat(uint key)
	{
		if (key > 1031)
			return false;
		return _keys[key] == KeyState::Repeat;
	}

	static bool clicked(uint button)
	{
		return pressed(button + 1024);
	}

	static bool clickRepeat(uint button)
	{
		return repeat(button + 1024);
	}
}
