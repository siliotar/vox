#pragma once

#include "Chunk.hpp"
#include <map>

class Map
{
private:
	std::unordered_map<int, Chunk>	_map;
public:
	Map();
	~Map();
	Map(Map& other) = delete;
	void operator=(const Map& other) = delete;

	Chunk* getChunk(int x, int y, int z);
	const Chunk* getChunk(int x, int y, int z) const;
	void setBlock(int x, int y, int z, uint id);
};
