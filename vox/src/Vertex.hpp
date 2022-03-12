#pragma once

#include "glm/glm.hpp"

/* 000000   11     111111111    11111   11111   11111 */
/*         light     texID      posZ    posY    posX  */

struct Vertex
{
	uint32_t data;

	Vertex() : data(0) {}

	Vertex(uint32_t data) : data(data) {}

	Vertex(uint8_t posX, uint16_t posY, uint8_t posZ, uint16_t texID, uint8_t light)
		: data(0)
	{
		data |= posX & 0b11111;
		data |= (posY & 0b11111) << 5;
		data |= (posZ & 0b11111) << 10;
		data |= (texID & 0b111111111) << 15;
		data |= (light & 0b11) << 24;
	}
};
