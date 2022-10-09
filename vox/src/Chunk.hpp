#pragma once

#define CHUNK_X 16
#define CHUNK_Y 16
#define CHUNK_Z 16

#include "Block.hpp"
#include "Vertex.hpp"
#include <vector>
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"

class Map;

class Chunk
{
private:
	const int	_x;
	const int	_y;
	const int	_z;
	const std::string _chunkCoordUniformName = "chunkCoord";
	int _chunkCoordUniformLocation;

	const Chunk* _leftChunk;
	const Chunk* _rightChunk;
	const Chunk* _backChunk;
	const Chunk* _frontChunk;
	const Chunk* _upChunk;
	const Chunk* _downChunk;

	VertexBuffer* _vb;
public:
	Block* blocks;
	bool modified;
	std::vector<Vertex> mesh;
	Chunk(int startX, int startY, int startZ);
	~Chunk();
	Chunk() = delete;
	Chunk(Chunk& other) = delete;
	void operator=(const Chunk& other) = delete;
	void calculateMesh(Map& map);
	void calculateGreedyMesh(Map& map);
	void draw(VertexArray& va, const VertexBufferLayout& vbLayout, Shader& shader);
	glm::ivec3 getPosition() const;
	void updateNeighbors();
};
