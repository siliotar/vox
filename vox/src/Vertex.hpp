#pragma once

#include "glm/glm.hpp"

/* 000000   11     111111111    11111   11111   11111 */
/*         light     texID      posZ    posY    posX  */

/*  111    111111111  |   11      1111      1111      1111    11111   11111   11111 */
/* Normal    texID    |  light   scaleZ    scaleY    scaleX   posZ    posY    posX  */

struct VertexData
{
	uint32_t	x;
	uint32_t	y;
};

struct Vertex
{
	VertexData data;

	Vertex() { memset(&data, 0, 4); }

	/*Vertex(uint8_t posX, uint8_t posY, uint8_t posZ, uint16_t texID, uint8_t light)
	{
		uint32_t tmp = 0;
		tmp |= posX & 0b11111;
		tmp |= (posY & 0b11111) << 5;
		tmp |= (posZ & 0b11111) << 10;
		tmp |= (texID & 0b111111111) << 15;
		tmp |= (light & 0b11) << 24;
		memcpy(&data, &tmp, 4);
	}*/

	Vertex(uint8_t posX, uint8_t posY, uint8_t posZ, uint16_t texID, uint8_t light, uint8_t scaleX, uint8_t scaleY, uint8_t scaleZ, uint8_t normal)
	{
		uint32_t tmp = 0;
		tmp |= posX & 0b11111;
		tmp |= (posY & 0b11111) << 5;
		tmp |= (posZ & 0b11111) << 10;
		tmp |= (scaleX & 0b11111) << 15;
		tmp |= (scaleY & 0b11111) << 20;
		tmp |= (scaleZ & 0b11111) << 25;
		tmp |= (light & 0b11) << 30;
		memcpy(&(data.x), &tmp, 4);
		tmp = 0;
		tmp |= texID & 0b111111111;
		tmp |= (normal & 0b111) << 9;
		memcpy(&(data.y), &tmp, 4);
	}
};
