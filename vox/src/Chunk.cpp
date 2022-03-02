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
	for (uint y = 0; y < CHUNK_Y; ++y)
	{
		for (uint z = 0; z < CHUNK_Z; z++)
		{
			for (uint x = 0; x < CHUNK_X; x++)
			{
				if (blocks[x + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)
					continue;
				uint texID = blocks[x + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].Texture;
				if (y == 0 || blocks[x + z * CHUNK_X + (y - 1) * CHUNK_Z * CHUNK_X].ID == 0)
				{
					mesh.emplace_back(x, y, z, 0, texID, 0);
					mesh.emplace_back(x + 1, y, z, 1, texID, 0);
					mesh.emplace_back(x + 1, y, z + 1, 2, texID, 0);
					mesh.emplace_back(x, y, z + 1, 3, texID, 0);
				}
				if (y == (CHUNK_Y - 1) || blocks[x + z * CHUNK_X + (y + 1) * CHUNK_Z * CHUNK_X].ID == 0)
				{
					mesh.emplace_back(x, y + 1, z + 1, 0, texID, 3);
					mesh.emplace_back(x + 1, y + 1, z + 1, 1, texID, 3);
					mesh.emplace_back(x + 1, y + 1, z, 2, texID, 3);
					mesh.emplace_back(x, y + 1, z, 3, texID, 3);
				}
				if ((z == 0 && (!front || front->blocks[x + (CHUNK_Z - 1) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (z != 0 && blocks[x + (z - 1) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
				{
					mesh.emplace_back(x + 1, y, z, 0, texID, 2);
					mesh.emplace_back(x, y, z, 1, texID, 2);
					mesh.emplace_back(x, y + 1, z, 2, texID, 2);
					mesh.emplace_back(x + 1, y + 1, z, 3, texID, 2);
				}
				if ((z == (CHUNK_Z - 1) && (!back || back->blocks[x + (0) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (z != (CHUNK_Z - 1) && blocks[x + (z + 1) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
				{
					mesh.emplace_back(x, y, z + 1, 0, texID, 2);
					mesh.emplace_back(x + 1, y, z + 1, 1, texID, 2);
					mesh.emplace_back(x + 1, y + 1, z + 1, 2, texID, 2);
					mesh.emplace_back(x, y + 1, z + 1, 3, texID, 2);
				}
				if ((x == 0 && (!right || right->blocks[(CHUNK_X - 1) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (x != 0 && blocks[(x - 1) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
				{
					mesh.emplace_back(x, y, z, 0, texID, 1);
					mesh.emplace_back(x, y, z + 1, 1, texID, 1);
					mesh.emplace_back(x, y + 1, z + 1, 2, texID, 1);
					mesh.emplace_back(x, y + 1, z, 3, texID, 1);
				}
				if ((x == (CHUNK_X - 1) && (!left || left->blocks[(0) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (x != (CHUNK_X - 1) && blocks[(x + 1) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
				{
					mesh.emplace_back(x + 1, y, z + 1, 0, texID, 1);
					mesh.emplace_back(x + 1, y, z, 1, texID, 1);
					mesh.emplace_back(x + 1, y + 1, z, 2, texID, 1);
					mesh.emplace_back(x + 1, y + 1, z + 1, 3, texID, 1);
				}
			}
		}
	}
	modified = false;
}

void Chunk::draw(VertexArray& va, const VertexBufferLayout& vbLayout, Shader& shader)
{
	VertexBuffer vb(mesh.data(), mesh.size() * sizeof(Vertex), GL_STATIC_DRAW);
	va.addBuffer(vb, vbLayout);
	shader.setUniform2f("chunkCoord", _x, _z);
	vb.bind();
	glDrawElements(GL_TRIANGLES, mesh.size() / 4 * 6, GL_UNSIGNED_INT, nullptr);
}
