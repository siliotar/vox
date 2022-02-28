#pragma once

#define CHUNK_X 16
#define CHUNK_Y 60
#define CHUNK_Z 16

#include "Block.hpp"

class Chunk
{
public:
	int	x;
	int	z;
	Block*	blocks;
	Chunk(int startX, int startZ);
	~Chunk();
	Chunk() = delete;
	Chunk(Chunk& other) = delete;
	void operator=(const Chunk& other) = delete;
};
