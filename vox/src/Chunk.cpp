#include "Chunk.hpp"
#include <cmath>
#include <iostream>

Chunk::Chunk(int startX, int startZ)
	: _x(startX), _z(startZ), blocks(nullptr), modified(true)
{
	blocks = new Block[CHUNK_X * CHUNK_Y * CHUNK_Z];

	for (size_t ty = 0; ty < CHUNK_Y; ++ty)
	{
		for (size_t tz = 0; tz < CHUNK_Z; tz++)
		{
			for (size_t tx = 0; tx < CHUNK_X; tx++)
			{
				if (ty > ((cos(abs(_x + (int)tx) * 0.6f) * 2.0f + 12.0f)) || ty > ((cos(abs(_z + (int)tz) * 0.6f) * 2.0f + 12.0f)))
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

void Chunk::calculateMesh(const Chunk* left, const Chunk* right, const Chunk* back, const Chunk* front)
{
	if (!modified)
		return;
	mesh.clear();
	for (size_t y = 0; y < CHUNK_Y; ++y)
	{
		for (size_t z = 0; z < CHUNK_Z; z++)
		{
			for (size_t x = 0; x < CHUNK_X; x++)
			{
				int	posX = x + _x;
				int	posY = y;
				int	posZ = z + _z;
				if (blocks[x + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)
					continue;
				uint texID = blocks[x + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].Texture;
				float	textureLeft = (float)(texID % TextureAtlasX) * TextureSize;
				float	textureDown = (float)(TextureAtlasY - 1 - texID / TextureAtlasY) * TextureSize;
				if (y == 0 || blocks[x + z * CHUNK_X + (y - 1) * CHUNK_Z * CHUNK_X].ID == 0)
				{
					mesh.emplace_back(posX, posY - 1.5f, posZ, textureLeft, textureDown);
					mesh.emplace_back(posX + 1.0f, posY - 1.5f, posZ, textureLeft + TextureSize, textureDown);
					mesh.emplace_back(posX + 1.0f, posY - 1.5f, posZ + 1.0f, textureLeft + TextureSize, textureDown + TextureSize);
					mesh.emplace_back(posX, posY - 1.5f, posZ + 1.0f, textureLeft, textureDown + TextureSize);
				}
				if (y == (CHUNK_Y - 1) || blocks[x + z * CHUNK_X + (y + 1) * CHUNK_Z * CHUNK_X].ID == 0)
				{
					mesh.emplace_back(posX, posY - 0.5f, posZ + 1.0f, textureLeft, textureDown);
					mesh.emplace_back(posX + 1.0f, posY - 0.5f, posZ + 1.0f, textureLeft + TextureSize, textureDown);
					mesh.emplace_back(posX + 1.0f, posY - 0.5f, posZ, textureLeft + TextureSize, textureDown + TextureSize);
					mesh.emplace_back(posX, posY - 0.5f, posZ, textureLeft, textureDown + TextureSize);
				}
				if ((z == 0 && (!front || front->blocks[x + (CHUNK_Z - 1) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (z != 0 && blocks[x + (z - 1) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
				{
					mesh.emplace_back(posX + 1.0f, posY - 1.5f, posZ, textureLeft, textureDown);
					mesh.emplace_back(posX, posY - 1.5f, posZ, textureLeft + TextureSize, textureDown);
					mesh.emplace_back(posX, posY - 0.5f, posZ, textureLeft + TextureSize, textureDown + TextureSize);
					mesh.emplace_back(posX + 1.0f, posY - 0.5f, posZ, textureLeft, textureDown + TextureSize);
				}
				if ((z == (CHUNK_Z - 1) && (!back || back->blocks[x + (0) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (z != (CHUNK_Z - 1) && blocks[x + (z + 1) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
				{
					mesh.emplace_back(posX, posY - 1.5f, posZ + 1.0f, textureLeft, textureDown);
					mesh.emplace_back(posX + 1.0f, posY - 1.5f, posZ + 1.0f, textureLeft + TextureSize, textureDown);
					mesh.emplace_back(posX + 1.0f, posY - 0.5f, posZ + 1.0f, textureLeft + TextureSize, textureDown + TextureSize);
					mesh.emplace_back(posX, posY - 0.5f, posZ + 1.0f, textureLeft, textureDown + TextureSize);
				}
				if ((x == 0 && (!right || right->blocks[(CHUNK_X - 1) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (x != 0 && blocks[(x - 1) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
				{
					mesh.emplace_back(posX, posY - 1.5f, posZ, textureLeft, textureDown);
					mesh.emplace_back(posX, posY - 1.5f, posZ + 1.0f, textureLeft + TextureSize, textureDown);
					mesh.emplace_back(posX, posY - 0.5f, posZ + 1.0f, textureLeft + TextureSize, textureDown + TextureSize);
					mesh.emplace_back(posX, posY - 0.5f, posZ, textureLeft, textureDown + TextureSize);
				}
				if ((x == (CHUNK_X - 1) && (!left || left->blocks[(0) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (x != (CHUNK_X - 1) && blocks[(x + 1) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
				{
					mesh.emplace_back(posX + 1.0f, posY - 1.5f, posZ + 1.0f, textureLeft, textureDown);
					mesh.emplace_back(posX + 1.0f, posY - 1.5f, posZ, textureLeft + TextureSize, textureDown);
					mesh.emplace_back(posX + 1.0f, posY - 0.5f, posZ, textureLeft + TextureSize, textureDown + TextureSize);
					mesh.emplace_back(posX + 1.0f, posY - 0.5f, posZ + 1.0f, textureLeft, textureDown + TextureSize);
				}
			}
		}
	}
	modified = false;
}
