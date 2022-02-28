#include "Map.hpp"
#include "Camera.hpp"

Map::Map(uint distance)
	: radius(distance)
{
	size_t size = radius * 2 + 1;
	void* rawMemory = operator new[](size * size * sizeof(Chunk));
	map = static_cast<Chunk*>(rawMemory);
	int	playerChunkX = static_cast<int>(Camera::getPlayerPosition().x) / CHUNK_X;
	int	playerChunkZ = static_cast<int>(Camera::getPlayerPosition().z) / CHUNK_Z;
	for (size_t i = 0; i < size; ++i)
	{
		for (size_t j = 0; j < size; ++j)
		{
			int	x = (playerChunkX - j + size / 2) * CHUNK_X;
			int z = (playerChunkZ - i + size / 2) * CHUNK_Z;
			new(&(map[j + i * size])) Chunk(x, z);
		}
	}
}

Map::~Map()
{}
