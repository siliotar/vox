#pragma once

#include "glm/glm.hpp"

struct Vertex
{
	glm::vec3	position;
	glm::vec2	texCoords;
	Vertex()
		: position(0.0f), texCoords(0.0f)
	{}
	Vertex(float xPos, float yPos, float zPos, float xTex, float yTex)
		: position(xPos, yPos, zPos), texCoords(xTex, yTex)
	{}
};
