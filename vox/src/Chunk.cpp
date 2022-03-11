#include "Chunk.hpp"
#include "Settings.hpp"
#include <cmath>
#include <iostream>

Chunk::Chunk(int startX, int startZ)
	: _x(startX), _z(startZ), blocks(nullptr), modified(true)
{
	blocks = new Block[CHUNK_X * CHUNK_Y * CHUNK_Z];

	float start = -MaxChunkCoord * CHUNK_X;

	for (size_t ty = 0; ty < CHUNK_Y; ++ty)
	{
		for (size_t tz = 0; tz < CHUNK_Z; tz++)
		{
			for (size_t tx = 0; tx < CHUNK_X; tx++)
			{
				//float xDist = float(_x + (int)tx) * 0.3f;
				//float zDist = float(_z + (int)tz) * 0.3f;
				//float dist = sqrtf(xDist * xDist + zDist * zDist);
				float x = start + _x + (int)tx;
				float z = start + _z + (int)tz;
				//if (ty > cos(dist) * 4.0f + 20.0f)
				//if (ty > ((cos(abs(_x + (int)tx) * 0.1f) * 6.0f + 22.0f)) || ty > ((cos(abs(_z + (int)tz) * 0.1f) * 6.0f + 22.0f)))
				if (ty > (((cos(x * 0.1f) + sin(z * 0.1f)) * 6.0f + 16.0f)))
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
				if (_x == 0 && _z == 9 * CHUNK_Z && x == 3 && z == 15 && y == 8)
					std::cout << "ku" << std::endl;
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

void Chunk::calculateGreedyMesh(const Chunk* left, const Chunk* right, const Chunk* back, const Chunk* front)
{
	if (!modified)
		return;
	mesh.clear();
	char upMask = 0b000001;
	char leftMask = 0b000010;
	char frontMask = 0b000100;
	char rightMask = 0b001000;
	char backMask = 0b010000;
	char downMask = 0b100000;
	char faces[CHUNK_X][CHUNK_Y][CHUNK_Z];
	memset(faces, 0, sizeof(faces));
	for (uint y = 0; y < CHUNK_Y; ++y)
	{
		for (uint z = 0; z < CHUNK_Z; z++)
		{
			for (uint x = 0; x < CHUNK_X; x++)
			{
				if (blocks[x + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)
					continue;
				if (y == 0 || blocks[x + z * CHUNK_X + (y - 1) * CHUNK_Z * CHUNK_X].ID == 0)
					faces[x][y][z] |= downMask;
				if (y == (CHUNK_Y - 1) || blocks[x + z * CHUNK_X + (y + 1) * CHUNK_Z * CHUNK_X].ID == 0)
					faces[x][y][z] |= upMask;
				if ((z == 0 && (!front || front->blocks[x + (CHUNK_Z - 1) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (z != 0 && blocks[x + (z - 1) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
					faces[x][y][z] |= frontMask;
				if ((z == (CHUNK_Z - 1) && (!back || back->blocks[x + (0) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (z != (CHUNK_Z - 1) && blocks[x + (z + 1) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
					faces[x][y][z] |= backMask;
				if ((x == 0 && (!right || right->blocks[(CHUNK_X - 1) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (x != 0 && blocks[(x - 1) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
					faces[x][y][z] |= rightMask;
				if ((x == (CHUNK_X - 1) && (!left || left->blocks[(0) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (x != (CHUNK_X - 1) && blocks[(x + 1) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
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
				if (faces[x][y][z] & downMask)
				{
					uint w = 0;
					for (uint tx = x; tx < CHUNK_X && (faces[tx][y][z] & downMask); ++tx)
					{
						faces[tx][y][z] &= ~downMask;
						++w;
					}
					uint h = 1;
					for (uint tz = z + 1; tz < CHUNK_Z && (faces[x][y][tz] & downMask); ++tz)
					{
						uint tw = 0;
						for (uint tx = x; tx < CHUNK_X && (faces[tx][y][tz] & downMask) && tw < w; ++tx)
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
					uint texID = 1;
					mesh.emplace_back(x, y, z, 0, texID, 0);
					mesh.emplace_back(x + w, y, z, 1, texID, 0);
					mesh.emplace_back(x + w, y, z + h, 2, texID, 0);
					mesh.emplace_back(x, y, z + h, 3, texID, 0);
				}
				if (faces[x][y][z] & upMask)
				{
					uint w = 0;
					for (uint tx = x; tx < CHUNK_X && (faces[tx][y][z] & upMask); ++tx)
					{
						faces[tx][y][z] &= ~upMask;
						++w;
					}
					uint h = 1;
					for (uint tz = z + 1; tz < CHUNK_Z && (faces[x][y][tz] & upMask); ++tz)
					{
						uint tw = 0;
						for (uint tx = x; tx < CHUNK_X && (faces[tx][y][tz] & upMask) && tw < w; ++tx)
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
					uint texID = 1;
					mesh.emplace_back(x, y + 1, z + h, 0, texID, 3);
					mesh.emplace_back(x + w, y + 1, z + h, 1, texID, 3);
					mesh.emplace_back(x + w, y + 1, z, 2, texID, 3);
					mesh.emplace_back(x, y + 1, z, 3, texID, 3);
				}
				if (faces[x][y][z] & frontMask)
				{
					uint w = 0;
					for (uint tx = x; tx < CHUNK_X && (faces[tx][y][z] & frontMask); ++tx)
					{
						faces[tx][y][z] &= ~frontMask;
						++w;
					}
					uint h = 1;
					for (uint ty = y + 1; ty < CHUNK_Y && (faces[x][ty][z] & frontMask); ++ty)
					{
						uint tw = 0;
						for (uint tx = x; tx < CHUNK_X && (faces[tx][ty][z] & frontMask) && tw < w; ++tx)
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
					uint texID = 1;
					mesh.emplace_back(x + w, y, z, 0, texID, 2);
					mesh.emplace_back(x, y, z, 1, texID, 2);
					mesh.emplace_back(x, y + h, z, 2, texID, 2);
					mesh.emplace_back(x + w, y + h, z, 3, texID, 2);
				}
				if (faces[x][y][z] & backMask)
				{
					uint w = 0;
					for (uint tx = x; tx < CHUNK_X && (faces[tx][y][z] & backMask); ++tx)
					{
						faces[tx][y][z] &= ~backMask;
						++w;
					}
					uint h = 1;
					for (uint ty = y + 1; ty < CHUNK_Y && (faces[x][ty][z] & backMask); ++ty)
					{
						uint tw = 0;
						for (uint tx = x; tx < CHUNK_X && (faces[tx][ty][z] & backMask) && tw < w; ++tx)
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
					uint texID = 1;
					mesh.emplace_back(x, y, z + 1, 0, texID, 2);
					mesh.emplace_back(x + w, y, z + 1, 1, texID, 2);
					mesh.emplace_back(x + w, y + h, z + 1, 2, texID, 2);
					mesh.emplace_back(x, y + h, z + 1, 3, texID, 2);
				}
				if (faces[x][y][z] & rightMask)
				{
					uint w = 0;
					for (uint tz = z; tz < CHUNK_Z && (faces[x][y][tz] & rightMask); ++tz)
					{
						faces[x][y][tz] &= ~rightMask;
						++w;
					}
					uint h = 1;
					for (uint ty = y + 1; ty < CHUNK_Y && (faces[x][ty][z] & rightMask); ++ty)
					{
						uint tw = 0;
						for (uint tz = z; tz < CHUNK_Z && (faces[x][ty][tz] & rightMask) && tw < w; ++tz)
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
					uint texID = 1;
					mesh.emplace_back(x, y, z, 0, texID, 1);
					mesh.emplace_back(x, y, z + w, 1, texID, 1);
					mesh.emplace_back(x, y + h, z + w, 2, texID, 1);
					mesh.emplace_back(x, y + h, z, 3, texID, 1);
				}
				if (faces[x][y][z] & leftMask)
				{
					uint w = 0;
					for (uint tz = z; tz < CHUNK_Z && (faces[x][y][tz] & leftMask); ++tz)
					{
						faces[x][y][tz] &= ~leftMask;
						++w;
					}
					uint h = 1;
					for (uint ty = y + 1; ty < CHUNK_Y && (faces[x][ty][z] & leftMask); ++ty)
					{
						uint tw = 0;
						for (uint tz = z; tz < CHUNK_Z && (faces[x][ty][tz] & leftMask) && tw < w; ++tz)
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
					uint texID = 1;
					mesh.emplace_back(x + 1, y, z + w, 0, texID, 1);
					mesh.emplace_back(x + 1, y, z, 1, texID, 1);
					mesh.emplace_back(x + 1, y + h, z, 2, texID, 1);
					mesh.emplace_back(x + 1, y + h, z + w, 3, texID, 1);
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
	shader.setUniform2i("chunkCoord", _x, _z);
	vb.bind();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, mesh.size() / 4 * 6, GL_UNSIGNED_INT, nullptr);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
