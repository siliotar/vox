#pragma once

#include "Chunk.hpp"
#include <map>
#include <functional>

class Map
{
private:
	Chunk** _map;
	Chunk** _tempBuffer;
	uint32_t _dataSize;
	int _size;

	int middleX;
	int middleY;
	int middleZ;

	void _loadMap();
	void _loadNewChunk(int x, int y, int z, int playerChunkX, int playerChunkY, int playerChunkZ);
public:
	Map();
	~Map();
	Map(Map& other) = delete;
	void operator=(const Map& other) = delete;

	Chunk* getChunk(int x, int y, int z);
	void setBlock(int x, int y, int z, uint id);

	void applyToAllChunks(std::function<void(Chunk*)> func);
	void updateMap();
};
