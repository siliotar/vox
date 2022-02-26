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
