#pragma once

#include "Chunk.hpp"
#include <map>

#define MaxChunkZ 1024

class Map
{
private:
	std::map<int, Chunk>	_map;
public:
	Map();
	~Map();
	Map(Map& other) = delete;
	void operator=(const Map& other) = delete;

	Chunk* getChunk(int x, int z);
	const Chunk* getChunk(int x, int z) const;
};
