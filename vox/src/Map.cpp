#include "Map.hpp"
#include "Camera.hpp"
#include "Settings.hpp"

Map::Map()
{
	int	playerChunkX = static_cast<int>(Camera::getPlayerPosition().x) / CHUNK_X;
	int	playerChunkY = static_cast<int>(Camera::getPlayerPosition().y) / CHUNK_Y;
	int	playerChunkZ = static_cast<int>(Camera::getPlayerPosition().z) / CHUNK_Z;
	for (int y = playerChunkY - RenderDistance; y <= playerChunkY + RenderDistance; ++y)
	{
		if (y < 0 || y >= MaxChunkHeight)
			continue;
		int yPos = y * 4 * MaxChunkWidth * MaxChunkWidth;
		for (int z = playerChunkZ - RenderDistance; z <= playerChunkZ + RenderDistance; ++z)
		{
			int zPos = (z + MaxChunkWidth) * MaxChunkWidth;
			for (int x = playerChunkX - RenderDistance; x <= playerChunkX + RenderDistance; ++x)
			{
				int pos = (x + MaxChunkWidth) + zPos + yPos;
				_map.try_emplace(pos, x * CHUNK_X, y * CHUNK_Y, z * CHUNK_Z);
			}
		}
	}
}

Map::~Map()
{}

Chunk* Map::getChunk(int x, int y, int z)
{
	if (y < 0 || y >= MaxChunkHeight)
		return nullptr;
	int pos = (x + MaxChunkWidth) + (z + MaxChunkWidth) * MaxChunkWidth + y * 4 * MaxChunkWidth * MaxChunkWidth;
	std::unordered_map<int, Chunk>::iterator it = _map.find(pos);
	if (it == _map.end())
		return &(_map.try_emplace(pos, x * CHUNK_X, y * CHUNK_Y, z * CHUNK_Z).first->second);
	return &it->second;
}

const Chunk* Map::getChunk(int x, int y, int z) const
{
	int pos = (x + MaxChunkWidth) + (z + MaxChunkWidth) * MaxChunkWidth + y * 4 * MaxChunkWidth * MaxChunkWidth;
	std::unordered_map<int, Chunk>::const_iterator it = _map.find(pos);
	if (y < 0 || y >= MaxChunkHeight || it == _map.end())
		return nullptr;
	return &it->second;
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
}
