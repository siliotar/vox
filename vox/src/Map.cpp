#include "Map.hpp"
#include "Camera.hpp"
#include "Settings.hpp"

Map::Map()
{
	int	playerChunkX = static_cast<int>(Camera::getPlayerPosition().x) / CHUNK_X;
	int	playerChunkZ = static_cast<int>(Camera::getPlayerPosition().z) / CHUNK_Z;
	for (int z = playerChunkZ - RenderDistance; z <= playerChunkZ + RenderDistance; ++z)
		for (int x = playerChunkX - RenderDistance; x <= playerChunkX + RenderDistance; ++x)
			_map.try_emplace(x + z * MaxChunkCoord, x * CHUNK_X, z * CHUNK_Z);
}

Map::~Map()
{}

Chunk* Map::getChunk(int x, int z)
{
	if (_map.find(x + z * MaxChunkCoord) == _map.end())
		_map.try_emplace(x + z * MaxChunkCoord, x * CHUNK_X, z * CHUNK_Z);
	return &_map.at(x + z * MaxChunkCoord);
}

const Chunk* Map::getChunk(int x, int z) const
{
	if (_map.find(x + z * MaxChunkCoord) != _map.end())
		return &_map.at(x + z * MaxChunkCoord);
	return nullptr;
}

void Map::setBlock(int x, int y, int z, uint id)
{
	if (y < 0 || y >= CHUNK_Y)
		return; // Error
	int chunkX = x / CHUNK_X;
	int chunkZ = z / CHUNK_Z;
	int xInChunk = x % CHUNK_X;
	int zInChunk = z % CHUNK_Z;
	if (x < 0 && xInChunk != 0)
		--chunkX;
	if (z < 0 && zInChunk != 0)
		--chunkZ;
	if (_map.find(chunkX + chunkZ * MaxChunkCoord) == _map.end())
		return; // Error
	if (xInChunk >= CHUNK_X || zInChunk >= CHUNK_Z)
		return; // Error
	if (xInChunk < 0)
		xInChunk = CHUNK_X + xInChunk;
	if (zInChunk < 0)
		zInChunk = CHUNK_Z + zInChunk;
	int pos = xInChunk + zInChunk * CHUNK_X + y * CHUNK_X * CHUNK_Z;
	_map.at(chunkX + chunkZ * MaxChunkCoord).blocks[pos].ID = id;
	_map.at(chunkX + chunkZ * MaxChunkCoord).blocks[pos].Texture = id; // Temp
}
