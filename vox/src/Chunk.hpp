#pragma once

#define CHUNK_X 16
#define CHUNK_Y 60
#define CHUNK_Z 16

#include "Block.hpp"
#include "Vertex.hpp"
#include <vector>
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"

class Chunk
{
private:
	int	_x;
	int	_z;
public:
	Block*	blocks;
	bool modified;
	std::vector<Vertex> mesh;
	Chunk(int startX, int startZ);
	~Chunk();
	Chunk() = delete;
	Chunk(Chunk& other) = delete;
	void operator=(const Chunk& other) = delete;
	void calculateMesh(const Chunk* left, const Chunk* right, const Chunk* back, const Chunk* front);
	void draw(VertexArray& va, const VertexBufferLayout& vbLayout, Shader& shader);
};
