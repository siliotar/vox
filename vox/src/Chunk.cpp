#include "Chunk.hpp"
#include <cmath>
#include <iostream>

Chunk::Chunk(int startX, int startZ)
	: x(startX), z(startZ), blocks(nullptr)
{
	blocks = new Block[CHUNK_X * CHUNK_Y * CHUNK_Z];

	for (size_t ty = 0; ty < CHUNK_Y; ++ty)
	{
		for (size_t tz = 0; tz < CHUNK_Z; tz++)
		{
			for (size_t tx = 0; tx < CHUNK_X; tx++)
			{
				if (ty > ((cos(abs(x + (int)tx) * 0.6f) * 2.0f + 12.0f)) || ty > ((cos(abs(z + (int)tz) * 0.6f) * 2.0f + 12.0f)))
					blocks[tx + tz * CHUNK_X + ty * CHUNK_Z * CHUNK_X].ID = 0;
				else if (ty < 7)
					blocks[tx + tz * CHUNK_X + ty * CHUNK_Z * CHUNK_X].ID = 1;
				else
					blocks[tx + tz * CHUNK_X + ty * CHUNK_Z * CHUNK_X].ID = 2;
				blocks[tx + tz * CHUNK_X + ty * CHUNK_Z * CHUNK_X].Texture = blocks[tx + tz * CHUNK_X + ty * CHUNK_Z * CHUNK_X].ID;
			}
		}
	}
}

Chunk::~Chunk()
{
	delete[] blocks;
}
