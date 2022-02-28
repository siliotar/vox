#pragma once

#include "Chunk.hpp"

class Map
{
public:
	uint	radius;
	Chunk*	map;
	Map(uint distance);
	~Map();
	Map() = delete;
	Map(Map& other) = delete;
	void operator=(const Map& other) = delete;
};
