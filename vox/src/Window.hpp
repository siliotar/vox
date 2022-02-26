#pragma once

#include "OpenGL.hpp"
#include <string>
#include <iostream>

class Window
{
public:
	Window() = delete;
	Window(Window& other) = delete;
	void operator=(const Window& other) = delete;

	static GLFWwindow* window;
	static int	width;
	static int	height;

	static void	init(int w, int h, const std::string& title);
	static bool	shouldClose();
	static void	swapBuffers();
	static void	terminate();
	static void shouldClose(bool flag);
	static void displayCursor();
	static void hideCursor();
};
