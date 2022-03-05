#include "Map.hpp"
#include "Camera.hpp"

Map::Map()
{
	int	playerChunkX = static_cast<int>(Camera::getPlayerPosition().x) / CHUNK_X;
	int	playerChunkZ = static_cast<int>(Camera::getPlayerPosition().z) / CHUNK_Z;
	for (int z = playerChunkZ - RenderDistance; z <= playerChunkZ + RenderDistance; ++z)
		for (int x = playerChunkX - RenderDistance; x <= playerChunkX + RenderDistance; ++x)
			_map.try_emplace(x + z * MaxChunkZ, x * CHUNK_X, z * CHUNK_Z);
}

Map::~Map()
{}

Chunk* Map::getChunk(int x, int z)
{
	if (_map.find(x + z * MaxChunkZ) == _map.end())
		_map.try_emplace(x + z * MaxChunkZ, x * CHUNK_X, z * CHUNK_Z);
	return &_map.at(x + z * MaxChunkZ);
}

const Chunk* Map::getChunk(int x, int z) const
{
	if (_map.find(x + z * MaxChunkZ) != _map.end())
		return &_map.at(x + z * MaxChunkZ);
	return nullptr;
}
