#include "Chunk.hpp"
#include "Settings.hpp"
#include "Map.hpp"
#include <cmath>
#include <iostream>
#include <glm/gtc/noise.hpp>
#include <PerlinNoise.hpp>

#define BLOCK_COORD(x, y, z) x + z * CHUNK_X + y * CHUNK_X * CHUNK_Z

Chunk::Chunk(int startX, int startY, int startZ)
	: _x(startX), _y(startY), _z(startZ), blocks(nullptr), modified(true), _vb(nullptr)
{
	blocks = new Block[CHUNK_X * CHUNK_Y * CHUNK_Z];

	float start = -MaxChunkWidth * CHUNK_X;

	for (size_t ty = 0; ty < CHUNK_Y; ++ty)
	{
		float y = float(_y + (int)ty);
		for (size_t tz = 0; tz < CHUNK_Z; tz++)
		{
			float z = start + _z + (int)tz;
			//float zsin = sin(z * 0.1f);
			for (size_t tx = 0; tx < CHUNK_X; tx++)
			{
				//float xDist = float(_x + (int)tx) * 0.3f;
				//float zDist = float(_z + (int)tz) * 0.3f;
				//float dist = sqrtf(xDist * xDist + zDist * zDist);
				float x = start + _x + (int)tx;
				//if (ty > cos(dist) * 4.0f + 20.0f)
				//if (ty > ((cos(abs(_x + (int)tx) * 0.1f) * 6.0f + 22.0f)) || ty > ((cos(abs(_z + (int)tz) * 0.1f) * 6.0f + 22.0f)))
				//if (y > (((cos(x * 0.1f) + zsin) * 6.0f + 16.0f)))
				float value = perlinNoise(1337, x / 128.0f, z / 128.0f) * 64.0f + 64.0f;
				if (y > value)
					blocks[BLOCK_COORD(tx, ty, tz)].ID = 0;
				else if (y < 7)
					blocks[BLOCK_COORD(tx, ty, tz)].ID = 1;
				else
					blocks[BLOCK_COORD(tx, ty, tz)].ID = 2;
			}
		}
	}
}

Chunk::~Chunk()
{
	delete[] blocks;
	delete _vb;
}

void Chunk::calculateMesh(Map& map)
{
	/*if (!modified)
		return;
	const Chunk* left = map.getChunk(_x / CHUNK_X + 1, _y / CHUNK_Y, _z / CHUNK_Z);
	const Chunk* right = map.getChunk(_x / CHUNK_X - 1, _y / CHUNK_Y, _z / CHUNK_Z);
	const Chunk* back = map.getChunk(_x / CHUNK_X, _y / CHUNK_Y, _z / CHUNK_Z + 1);
	const Chunk* front = map.getChunk(_x / CHUNK_X, _y / CHUNK_Y, _z / CHUNK_Z - 1);
	const Chunk* up = map.getChunk(_x / CHUNK_X, _y / CHUNK_Y + 1, _z / CHUNK_Z);
	const Chunk* down = map.getChunk(_x / CHUNK_X, _y / CHUNK_Y - 1, _z / CHUNK_Z);
	mesh.clear();
	for (uint y = 0; y < CHUNK_Y; ++y)
	{
		for (uint z = 0; z < CHUNK_Z; z++)
		{
			for (uint x = 0; x < CHUNK_X; x++)
			{
				if (blocks[BLOCK_COORD(x, y, z)].ID == 0)
					continue;
				uint texID = blocks[BLOCK_COORD(x, y, z)].Texture;
				if (y == 0 && (!down || down->blocks[BLOCK_COORD(x, (CHUNK_Y - 1), z)].ID == 0) \
					|| (y != 0 && blocks[BLOCK_COORD(x, (y - 1), z)].ID == 0))
				{
					mesh.emplace_back(x, y, z, texID, 0);
					mesh.emplace_back(x + 1, y, z, texID, 0);
					mesh.emplace_back(x + 1, y, z + 1, texID, 0);
					mesh.emplace_back(x, y, z + 1, texID, 0);
				}
				if (y == (CHUNK_Y - 1) && (!up || up->blocks[BLOCK_COORD(x, 0, z)].ID == 0) \
					|| (y != (CHUNK_Y - 1) && blocks[BLOCK_COORD(x, (y + 1), z)].ID == 0))
				{
					mesh.emplace_back(x, y + 1, z + 1, texID, 3);
					mesh.emplace_back(x + 1, y + 1, z + 1, texID, 3);
					mesh.emplace_back(x + 1, y + 1, z, texID, 3);
					mesh.emplace_back(x, y + 1, z, texID, 3);
				}
				if ((z == 0 && (!front || front->blocks[BLOCK_COORD(x, y, (CHUNK_Z - 1))].ID == 0)) \
					|| (z != 0 && blocks[BLOCK_COORD(x, y, (z - 1))].ID == 0))
				{
					mesh.emplace_back(x + 1, y, z, texID, 2);
					mesh.emplace_back(x, y, z, texID, 2);
					mesh.emplace_back(x, y + 1, z, texID, 2);
					mesh.emplace_back(x + 1, y + 1, z, texID, 2);
				}
				if ((z == (CHUNK_Z - 1) && (!back || back->blocks[BLOCK_COORD(x, y, 0)].ID == 0)) \
					|| (z != (CHUNK_Z - 1) && blocks[BLOCK_COORD(x, y, (z + 1))].ID == 0))
				{
					mesh.emplace_back(x, y, z + 1, texID, 2);
					mesh.emplace_back(x + 1, y, z + 1, texID, 2);
					mesh.emplace_back(x + 1, y + 1, z + 1, texID, 2);
					mesh.emplace_back(x, y + 1, z + 1, texID, 2);
				}
				if ((x == 0 && (!right || right->blocks[BLOCK_COORD((CHUNK_X - 1), y, z)].ID == 0)) \
					|| (x != 0 && blocks[BLOCK_COORD((x - 1), y, z)].ID == 0))
				{
					mesh.emplace_back(x, y, z, texID, 1);
					mesh.emplace_back(x, y, z + 1, texID, 1);
					mesh.emplace_back(x, y + 1, z + 1, texID, 1);
					mesh.emplace_back(x, y + 1, z, texID, 1);
				}
				if ((x == (CHUNK_X - 1) && (!left || left->blocks[BLOCK_COORD(0, y, z)].ID == 0)) \
					|| (x != (CHUNK_X - 1) && blocks[BLOCK_COORD((x + 1), y, z)].ID == 0))
				{
					mesh.emplace_back(x + 1, y, z + 1, texID, 1);
					mesh.emplace_back(x + 1, y, z, texID, 1);
					mesh.emplace_back(x + 1, y + 1, z, texID, 1);
					mesh.emplace_back(x + 1, y + 1, z + 1, texID, 1);
				}
			}
		}
	}
	modified = false;*/
}

void Chunk::calculateGreedyMesh(Map& map)
{
	if (!modified)
		return;
	const Chunk* left = map.getChunk(_x / CHUNK_X + 1, _y / CHUNK_Y, _z / CHUNK_Z);
	const Chunk* right = map.getChunk(_x / CHUNK_X - 1, _y / CHUNK_Y, _z / CHUNK_Z);
	const Chunk* back = map.getChunk(_x / CHUNK_X, _y / CHUNK_Y, _z / CHUNK_Z + 1);
	const Chunk* front = map.getChunk(_x / CHUNK_X, _y / CHUNK_Y, _z / CHUNK_Z - 1);
	const Chunk* up = map.getChunk(_x / CHUNK_X, _y / CHUNK_Y + 1, _z / CHUNK_Z);
	const Chunk* down = map.getChunk(_x / CHUNK_X, _y / CHUNK_Y - 1, _z / CHUNK_Z);
	mesh.clear();
	char upMask = 0b000001;
	char leftMask = 0b000010;
	char frontMask = 0b000100;
	char rightMask = 0b001000;
	char backMask = 0b010000;
	char downMask = 0b100000;
	char faces[CHUNK_X][CHUNK_Y][CHUNK_Z] = { 0 };
	for (uint y = 0; y < CHUNK_Y; ++y)
	{
		for (uint z = 0; z < CHUNK_Z; z++)
		{
			for (uint x = 0; x < CHUNK_X; x++)
			{
				if (blocks[BLOCK_COORD(x, y, z)].ID == 0)
					continue;
				if (y == 0 && (!down || down->blocks[BLOCK_COORD(x, (CHUNK_Y - 1), z)].ID == 0) \
					|| (y != 0 && blocks[BLOCK_COORD(x, (y - 1), z)].ID == 0))
					faces[x][y][z] |= downMask;
				if (y == (CHUNK_Y - 1) && (!up || up->blocks[BLOCK_COORD(x, 0, z)].ID == 0) \
					|| (y != (CHUNK_Y - 1) && blocks[BLOCK_COORD(x, (y + 1), z)].ID == 0))
					faces[x][y][z] |= upMask;
				if ((z == 0 && (!front || front->blocks[BLOCK_COORD(x, y, (CHUNK_Z - 1))].ID == 0)) \
					|| (z != 0 && blocks[BLOCK_COORD(x, y, (z - 1))].ID == 0))
					faces[x][y][z] |= frontMask;
				if ((z == (CHUNK_Z - 1) && (!back || back->blocks[BLOCK_COORD(x, y, 0)].ID == 0)) \
					|| (z != (CHUNK_Z - 1) && blocks[BLOCK_COORD(x, y, (z + 1))].ID == 0))
					faces[x][y][z] |= backMask;
				if ((x == 0 && (!right || right->blocks[BLOCK_COORD((CHUNK_X - 1), y, z)].ID == 0)) \
					|| (x != 0 && blocks[BLOCK_COORD((x - 1), y, z)].ID == 0))
					faces[x][y][z] |= rightMask;
				if ((x == (CHUNK_X - 1) && (!left || left->blocks[BLOCK_COORD(0, y, z)].ID == 0)) \
					|| (x != (CHUNK_X - 1) && blocks[BLOCK_COORD((x + 1), y, z)].ID == 0))
					faces[x][y][z] |= leftMask;
			}
		}
	}
	for (uint y = 0; y < CHUNK_Y; ++y)
	{
		for (uint z = 0; z < CHUNK_Z; z++)
		{
			for (uint x = 0; x < CHUNK_X; x++)
			{
				if (faces[x][y][z] == 0)
					continue;
				uint texID = blocks[BLOCK_COORD(x, y, z)].ID;
				if (faces[x][y][z] & downMask)
				{
					uint w = 0;
					for (uint tx = x; tx < CHUNK_X && (faces[tx][y][z] & downMask) && blocks[BLOCK_COORD(tx, y, z)].ID == texID; ++tx)
					{
						faces[tx][y][z] &= ~downMask;
						++w;
					}
					uint h = 1;
					for (uint tz = z + 1; tz < CHUNK_Z && (faces[x][y][tz] & downMask) && blocks[BLOCK_COORD(x, y, tz)].ID == texID; ++tz)
					{
						uint tw = 0;
						for (uint tx = x; tx < CHUNK_X && (faces[tx][y][tz] & downMask) && blocks[BLOCK_COORD(tx, y, tz)].ID == texID && tw < w; ++tx)
							++tw;
						if (tw >= w)
						{
							++h;
							for (uint tx = x; tx < x + w; ++tx)
								faces[tx][y][tz] &= ~downMask;
						}
						else
							break;
					}
					mesh.emplace_back(x, y, z, texID, 0, w, 1, h, 5);
				}
				if (faces[x][y][z] & upMask)
				{
					uint w = 0;
					for (uint tx = x; tx < CHUNK_X && (faces[tx][y][z] & upMask) && blocks[BLOCK_COORD(tx, y, z)].ID == texID; ++tx)
					{
						faces[tx][y][z] &= ~upMask;
						++w;
					}
					uint h = 1;
					for (uint tz = z + 1; tz < CHUNK_Z && (faces[x][y][tz] & upMask) && blocks[BLOCK_COORD(x, y, tz)].ID == texID; ++tz)
					{
						uint tw = 0;
						for (uint tx = x; tx < CHUNK_X && (faces[tx][y][tz] & upMask) && blocks[BLOCK_COORD(tx, y, tz)].ID == texID && tw < w; ++tx)
							++tw;
						if (tw >= w)
						{
							++h;
							for (uint tx = x; tx < x + w; ++tx)
								faces[tx][y][tz] &= ~upMask;
						}
						else
							break;
					}
					mesh.emplace_back(x, y, z, texID, 3, w, 1, h, 0);
				}
				if (faces[x][y][z] & frontMask)
				{
					uint w = 0;
					for (uint tx = x; tx < CHUNK_X && (faces[tx][y][z] & frontMask) && blocks[BLOCK_COORD(tx, y, z)].ID == texID; ++tx)
					{
						faces[tx][y][z] &= ~frontMask;
						++w;
					}
					uint h = 1;
					for (uint ty = y + 1; ty < CHUNK_Y && (faces[x][ty][z] & frontMask) && blocks[BLOCK_COORD(x, ty, z)].ID == texID; ++ty)
					{
						uint tw = 0;
						for (uint tx = x; tx < CHUNK_X && (faces[tx][ty][z] & frontMask) && blocks[BLOCK_COORD(tx, ty, z)].ID == texID && tw < w; ++tx)
							++tw;
						if (tw >= w)
						{
							++h;
							for (uint tx = x; tx < x + w; ++tx)
								faces[tx][ty][z] &= ~frontMask;
						}
						else
							break;
					}
					mesh.emplace_back(x, y, z, texID, 2, w, h, 1, 2);
				}
				if (faces[x][y][z] & backMask)
				{
					uint w = 0;
					for (uint tx = x; tx < CHUNK_X && (faces[tx][y][z] & backMask) && blocks[BLOCK_COORD(tx, y, z)].ID == texID; ++tx)
					{
						faces[tx][y][z] &= ~backMask;
						++w;
					}
					uint h = 1;
					for (uint ty = y + 1; ty < CHUNK_Y && (faces[x][ty][z] & backMask) && blocks[BLOCK_COORD(x, ty, z)].ID == texID; ++ty)
					{
						uint tw = 0;
						for (uint tx = x; tx < CHUNK_X && (faces[tx][ty][z] & backMask) && blocks[BLOCK_COORD(tx, ty, z)].ID == texID && tw < w; ++tx)
							++tw;
						if (tw >= w)
						{
							++h;
							for (uint tx = x; tx < x + w; ++tx)
								faces[tx][ty][z] &= ~backMask;
						}
						else
							break;
					}
					mesh.emplace_back(x, y, z, texID, 2, w, h, 1, 4);
				}
				if (faces[x][y][z] & rightMask)
				{
					uint w = 0;
					for (uint tz = z; tz < CHUNK_Z && (faces[x][y][tz] & rightMask) && blocks[BLOCK_COORD(x, y, tz)].ID == texID; ++tz)
					{
						faces[x][y][tz] &= ~rightMask;
						++w;
					}
					uint h = 1;
					for (uint ty = y + 1; ty < CHUNK_Y && (faces[x][ty][z] & rightMask) && blocks[BLOCK_COORD(x, ty, z)].ID == texID; ++ty)
					{
						uint tw = 0;
						for (uint tz = z; tz < CHUNK_Z && (faces[x][ty][tz] & rightMask) && blocks[BLOCK_COORD(x, ty, tz)].ID == texID && tw < w; ++tz)
							++tw;
						if (tw >= w)
						{
							++h;
							for (uint tz = z; tz < z + w; ++tz)
								faces[x][ty][tz] &= ~rightMask;
						}
						else
							break;
					}
					mesh.emplace_back(x, y, z, texID, 1, 1, h, w, 3);
				}
				if (faces[x][y][z] & leftMask)
				{
					uint w = 0;
					for (uint tz = z; tz < CHUNK_Z && (faces[x][y][tz] & leftMask) && blocks[BLOCK_COORD(x, y, tz)].ID == texID; ++tz)
					{
						faces[x][y][tz] &= ~leftMask;
						++w;
					}
					uint h = 1;
					for (uint ty = y + 1; ty < CHUNK_Y && (faces[x][ty][z] & leftMask) && blocks[BLOCK_COORD(x, ty, z)].ID == texID; ++ty)
					{
						uint tw = 0;
						for (uint tz = z; tz < CHUNK_Z && (faces[x][ty][tz] & leftMask) && blocks[BLOCK_COORD(x, ty, tz)].ID == texID && tw < w; ++tz)
							++tw;
						if (tw >= w)
						{
							++h;
							for (uint tz = z; tz < z + w; ++tz)
								faces[x][ty][tz] &= ~leftMask;
						}
						else
							break;
					}
					mesh.emplace_back(x, y, z, texID, 1, 1, h, w, 1);
				}
			}
		}
	}
	delete _vb;
	_vb = nullptr;
	modified = false;
}

void Chunk::draw(VertexArray& va, const VertexBufferLayout& vbLayout, Shader& shader)
{
	size_t meshSize = mesh.size();
	if (meshSize == 0)
		return;
	if (_vb == nullptr)
		_vb = new VertexBuffer(mesh.data(), meshSize * sizeof(Vertex), GL_STATIC_DRAW);
	va.addBuffer(*_vb, vbLayout);
	shader.setUniform3i(_chunkCoordUniformName, _x, _y, _z);
	_vb->bind();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, meshSize);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
