#pragma once

#include "glm/glm.hpp"

/*   1111      1111      1111    111111111  |   111      11      1111      1111      1111    11111   11111   11111 */
/* clusterZ  clusterY  clusterX    texID    |  normal   light   scaleZ    scaleY    scaleX   posZ    posY    posX  */

struct VertexData
{
	uint32_t	x;
	uint32_t	y;
};
#include <iostream>
struct Vertex
{
	VertexData data;

	Vertex() { memset(&data, 0, sizeof(VertexData)); }

	Vertex(uint8_t posX, uint8_t posY, uint8_t posZ, uint16_t texID, uint8_t light, uint8_t scaleX, uint8_t scaleY, uint8_t scaleZ, uint8_t normal)
	{
		uint32_t tmp = 0;
		tmp |= posX & 0b11111;
		tmp |= (posY & 0b11111) << 5;
		tmp |= (posZ & 0b11111) << 10;
		tmp |= ((scaleX - 1) & 0b1111) << 15;
		tmp |= ((scaleY - 1) & 0b1111) << 19;
		tmp |= ((scaleZ - 1) & 0b1111) << 23;
		tmp |= (light & 0b11) << 27;
		tmp |= (normal & 0b111) << 29;
		memcpy(&(data.x), &tmp, 4);
		tmp = 0;
		tmp |= texID & 0b111111111;
		//tmp |= (clusterX & 0b1111) << 9;
		//tmp |= (clusterY & 0b1111) << 13;
		//tmp |= (clusterZ & 0b1111) << 17;
		memcpy(&(data.y), &tmp, 4);
	}
};
