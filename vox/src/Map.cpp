#include "Map.hpp"
#include "Camera.hpp"
#include "Settings.hpp"

Map::Map()
{
	int	playerChunkX = static_cast<int>(Camera::getPlayerPosition().x) / CHUNK_X;
	int	playerChunkY = static_cast<int>(Camera::getPlayerPosition().y) / CHUNK_Y;
	int	playerChunkZ = static_cast<int>(Camera::getPlayerPosition().z) / CHUNK_Z;
	for (int y = playerChunkY - RenderDistance; y <= playerChunkY + RenderDistance; ++y)
		for (int z = playerChunkZ - RenderDistance; z <= playerChunkZ + RenderDistance; ++z)
			for (int x = playerChunkX - RenderDistance; x <= playerChunkX + RenderDistance; ++x)
			{
				if (y < 0 || y >= MaxChunkHeight)
					continue;
				int pos = (x + MaxChunkWidth) + (z + MaxChunkWidth) * MaxChunkWidth + y * 4 * MaxChunkWidth * MaxChunkWidth;
				_map.try_emplace(pos, x * CHUNK_X, y * CHUNK_Y, z * CHUNK_Z);
			}
}

Map::~Map()
{}

Chunk* Map::getChunk(int x, int y, int z)
{
	if (y < 0 || y >= MaxChunkHeight)
		return nullptr;
	int pos = (x + MaxChunkWidth) + (z + MaxChunkWidth) * MaxChunkWidth + y * 4 * MaxChunkWidth * MaxChunkWidth;
	if (_map.count(pos) == 0)
		_map.try_emplace(pos, x * CHUNK_X, y * CHUNK_Y, z * CHUNK_Z);
	return &_map.at(pos);
}

const Chunk* Map::getChunk(int x, int y, int z) const
{
	int pos = (x + MaxChunkWidth) + (z + MaxChunkWidth) * MaxChunkWidth + y * 4 * MaxChunkWidth * MaxChunkWidth;
	if (y < 0 || y >= MaxChunkHeight || _map.find(pos) == _map.end())
		return nullptr;
	return &_map.at(pos);
}

void Map::setBlock(int x, int y, int z, uint id)
{
	if (y < 0 || y >= CHUNK_Y * MaxChunkHeight)
		return; // Error
	int chunkX = (x + MaxChunkWidth * CHUNK_X) / CHUNK_X;
	int chunkY = y / CHUNK_Y;
	int chunkZ = (z + MaxChunkWidth * CHUNK_Z) / CHUNK_Z;
	int xInChunk = x % CHUNK_X;
	int yInChunk = y % CHUNK_Y;
	int zInChunk = z % CHUNK_Z;
	int pos = chunkX + chunkZ * MaxChunkWidth + chunkY * 4 * MaxChunkWidth * MaxChunkWidth;
	if (_map.find(pos) == _map.end())
		return; // Error
	if (xInChunk >= CHUNK_X || yInChunk >= CHUNK_Y || zInChunk >= CHUNK_Z)
		return; // Error
	if (xInChunk < 0)
		xInChunk = CHUNK_X + xInChunk;
	if (zInChunk < 0)
		zInChunk = CHUNK_Z + zInChunk;
	_map.at(pos).blocks[xInChunk + zInChunk * CHUNK_X + yInChunk * CHUNK_X * CHUNK_Z].ID = id;
	_map.at(pos).blocks[xInChunk + zInChunk * CHUNK_X + yInChunk * CHUNK_X * CHUNK_Z].Texture = id; // Temp
}
