#pragma once

#ifndef _WIN32
# include <glad/glad.h>
#else
# define GLEW_STATIC
# include <GL/glew.h>
# include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>

typedef unsigned int uint;

#define ASSERT(x) if (!(x)) __debugbreak;
#define GLCall(x) { GLClearError(); \
	x; \
	ASSERT(GLLogCall(#x, __FILE__, __LINE__)) }

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);
