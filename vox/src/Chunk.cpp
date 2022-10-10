#include "Chunk.hpp"
#include "Settings.hpp"
#include "Map.hpp"
#include <cmath>
#include <iostream>
#include <glm/gtc/noise.hpp>
#include <PerlinNoise.hpp>

#define BLOCK_COORD(x, y, z) x + z * CHUNK_X + y * CHUNK_X * CHUNK_Z

Chunk::Chunk(int startX, int startY, int startZ)
	: _x(startX), _y(startY), _z(startZ), blocks(nullptr), modified(true), _vb(nullptr), _chunkCoordUniformLocation(-1),
	_leftChunk(nullptr), _rightChunk(nullptr), _backChunk(nullptr), _frontChunk(nullptr), _upChunk(nullptr), _downChunk(nullptr),
	_updateVB(true), _meshSize(0), _aabb({ startX, startY, startZ }, { startX + CHUNK_X, startY + CHUNK_Y, startZ + CHUNK_Z })
{
	blocks = new Block[CHUNK_X * CHUNK_Y * CHUNK_Z];

	int start = -MaxChunkWidth * CHUNK_X;

	int seed = 1337;

	int noise[CHUNK_X * CHUNK_Z];
	int worldStartX = start + _x;
	int worldStartZ = start + _z;
	float scale = 1.0f / 128.0f;

	for (int tz = 0; tz < CHUNK_Z; ++tz)
		for (int tx = 0; tx < CHUNK_X; ++tx)
			noise[tx + tz * CHUNK_X] = static_cast<int>(perlinNoise(seed, float(worldStartX + tx) * scale, float(worldStartZ + tz) * scale) * 64.0f) + 64;

	for (int ty = 0; ty < CHUNK_Y; ++ty)
	{
		int y = _y + ty;
		for (int tz = 0; tz < CHUNK_Z; tz++)
		{
			for (int tx = 0; tx < CHUNK_X; tx++)
			{
				int value = noise[tx + tz * CHUNK_X];
				if (y > value)
					blocks[BLOCK_COORD(tx, ty, tz)].ID = 0;
				else if (y + 1 > value)
					blocks[BLOCK_COORD(tx, ty, tz)].ID = 2;
				else
					blocks[BLOCK_COORD(tx, ty, tz)].ID = 1;
			}
		}
	}
}

Chunk::~Chunk()
{
	delete[] blocks;
	delete _vb;
}

void Chunk::calculateGreedyMesh(Map& map)
{
	if (!modified)
		return;
	if (_leftChunk == nullptr)
		_leftChunk = map.getChunk(_x / CHUNK_X + 1, _y / CHUNK_Y, _z / CHUNK_Z);
	if (_rightChunk == nullptr)
		_rightChunk = map.getChunk(_x / CHUNK_X - 1, _y / CHUNK_Y, _z / CHUNK_Z);
	if (_backChunk == nullptr)
		_backChunk = map.getChunk(_x / CHUNK_X, _y / CHUNK_Y, _z / CHUNK_Z + 1);
	if (_frontChunk == nullptr)
		_frontChunk = map.getChunk(_x / CHUNK_X, _y / CHUNK_Y, _z / CHUNK_Z - 1);
	if (_upChunk == nullptr)
		_upChunk = map.getChunk(_x / CHUNK_X, _y / CHUNK_Y + 1, _z / CHUNK_Z);
	if (_downChunk == nullptr)
		_downChunk = map.getChunk(_x / CHUNK_X, _y / CHUNK_Y - 1, _z / CHUNK_Z);
	mesh.clear();
	const char upMask = 0b000001;
	const char leftMask = 0b000010;
	const char frontMask = 0b000100;
	const char rightMask = 0b001000;
	const char backMask = 0b010000;
	const char downMask = 0b100000;
	char faces[CHUNK_X][CHUNK_Y][CHUNK_Z] = { 0 };
	for (uint y = 0; y < CHUNK_Y; ++y)
	{
		for (uint z = 0; z < CHUNK_Z; z++)
		{
			for (uint x = 0; x < CHUNK_X; x++)
			{
				if (blocks[BLOCK_COORD(x, y, z)].ID == 0)
					continue;
				if (y == 0 && (_downChunk && _downChunk->blocks[BLOCK_COORD(x, (CHUNK_Y - 1), z)].ID == 0) \
					|| (y != 0 && blocks[BLOCK_COORD(x, (y - 1), z)].ID == 0))
					faces[x][y][z] |= downMask;
				if (y == (CHUNK_Y - 1) && (_upChunk && _upChunk->blocks[BLOCK_COORD(x, 0, z)].ID == 0) \
					|| (y != (CHUNK_Y - 1) && blocks[BLOCK_COORD(x, (y + 1), z)].ID == 0))
					faces[x][y][z] |= upMask;
				if ((z == 0 && (_frontChunk && _frontChunk->blocks[BLOCK_COORD(x, y, (CHUNK_Z - 1))].ID == 0)) \
					|| (z != 0 && blocks[BLOCK_COORD(x, y, (z - 1))].ID == 0))
					faces[x][y][z] |= frontMask;
				if ((z == (CHUNK_Z - 1) && (_backChunk && _backChunk->blocks[BLOCK_COORD(x, y, 0)].ID == 0)) \
					|| (z != (CHUNK_Z - 1) && blocks[BLOCK_COORD(x, y, (z + 1))].ID == 0))
					faces[x][y][z] |= backMask;
				if ((x == 0 && (_rightChunk && _rightChunk->blocks[BLOCK_COORD((CHUNK_X - 1), y, z)].ID == 0)) \
					|| (x != 0 && blocks[BLOCK_COORD((x - 1), y, z)].ID == 0))
					faces[x][y][z] |= rightMask;
				if ((x == (CHUNK_X - 1) && (_leftChunk && _leftChunk->blocks[BLOCK_COORD(0, y, z)].ID == 0)) \
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
	_meshSize = mesh.size();
	_updateVB = true;
	modified = false;
}

void Chunk::draw(VertexArray& va, const VertexBufferLayout& vbLayout, Shader& shader)
{
	if (_meshSize == 0)
		return;
	if (_updateVB)
	{
		delete _vb;
		_vb = new VertexBuffer(mesh.data(), _meshSize * sizeof(Vertex), GL_STATIC_DRAW);
		_updateVB = false;
	}
	va.addBuffer(*_vb, vbLayout);
	if (_chunkCoordUniformLocation == -1)
		_chunkCoordUniformLocation = shader.getUniformLocation(_chunkCoordUniformName);
	shader.setUniform3i(_chunkCoordUniformLocation, _x, _y, _z);
	_vb->bind();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, _meshSize);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

glm::ivec3 Chunk::getPosition() const
{
	return { _x, _y, _z };
}

void Chunk::updateNeighbors()
{
	_leftChunk = nullptr;
	_rightChunk = nullptr;
	_backChunk = nullptr;
	_frontChunk = nullptr;
	_upChunk = nullptr;
	_downChunk = nullptr;
	modified = true;
}

bool Chunk::isCulled(const Frustum& camFrustum) const
{
	return !_aabb.isOnFrustum(camFrustum);
}
