#include "Map.hpp"
#include "Camera.hpp"
#include "Settings.hpp"

int calculateMapSize()
{
	int result = 0;

	int distance2 = (RenderDistance + 1) * (RenderDistance + 1);
	int size = RenderDistance * 2 + 1;
	for (int y = 0; y < MaxChunkHeight; ++y)
	{
		for (int z = 0; z < size; ++z)
		{
			int distZ = RenderDistance - z;
			int distZ2 = distZ * distZ;
			for (int x = 0; x < size; ++x)
			{
				int distX = RenderDistance - x;
				int distX2 = distX * distX;
				if (distX2 + distZ2 > distance2)
					continue;
				++result;
			}
		}
	}
	return result;
}

void Map::_loadMap()
{
	_size = RenderDistance * 2 + 1;
	const glm::vec3& playerPos = Camera::getPlayerPosition();

	int	playerChunkY = static_cast<int>(playerPos.y) / CHUNK_Y;
	if (playerChunkY - RenderDistance >= MaxChunkHeight || playerChunkY + RenderDistance < 0)
		return;

	int	playerChunkX = static_cast<int>(playerPos.x) / CHUNK_X;
	int	playerChunkZ = static_cast<int>(playerPos.z) / CHUNK_Z;

	int distance2 = (RenderDistance + 1) * (RenderDistance + 1);
	int maxY = playerChunkY + RenderDistance;
	int startY = playerChunkY - RenderDistance;
	if (startY < 0)
		startY = 0;
	if (maxY >= MaxChunkHeight)
		maxY = MaxChunkHeight - 1;
	for (int y = startY; y <= maxY; ++y)
	{
		int tempY = y - startY;
		for (int z = 0; z < _size; ++z)
		{
			int chunkZpos = (z - RenderDistance + playerChunkZ) * CHUNK_Z;
			int distZ = RenderDistance - z;
			int distZ2 = distZ * distZ;
			for (int x = 0; x < _size; ++x)
			{
				int distX = RenderDistance - x;
				int distX2 = distX * distX;
				if (distX2 + distZ2 > distance2)
					continue;
				int chunkXpos = (x - RenderDistance + playerChunkX) * CHUNK_X;
				_map[x + z * _size + tempY * _size * _size] = new Chunk(chunkXpos, y * CHUNK_Y, chunkZpos);
			}
		}
	}

	middleX = playerChunkX;
	middleY = playerChunkY;
	middleZ = playerChunkZ;
}

Map::Map()
{
	_size = RenderDistance * 2 + 1;
	_map = new Chunk * [_size * _size * MaxChunkHeight];
	_tempBuffer = new Chunk * [_size * _size * MaxChunkHeight];
	_dataSize = _size * _size * MaxChunkHeight * sizeof(Chunk*);
	memset(_map, 0, _dataSize);

	_loadMap();
}

Map::~Map()
{}

Chunk* Map::getChunk(int x, int y, int z)
{
	const glm::vec3& playerPos = Camera::getPlayerPosition();
	x -= static_cast<int>(playerPos.x) / CHUNK_X - RenderDistance;
	z -= static_cast<int>(playerPos.z) / CHUNK_Z - RenderDistance;
	//y -= static_cast<int>(playerPos.y) / CHUNK_Y - RenderDistance;
	int tempY = y - static_cast<int>(playerPos.y) / CHUNK_Y - RenderDistance;
	if (y < 0 || y >= MaxChunkHeight || tempY >= _size || x < 0 || x >= _size || z < 0 || z >= _size)
		return nullptr;
	return (_map[x + z * _size + y * _size * _size]);
}

void Map::setBlock(int x, int y, int z, uint id)
{
	if (y < 0 || y >= CHUNK_Y * MaxChunkHeight)
		return; // Error
	int chunkX = x / CHUNK_X;
	int chunkY = y / CHUNK_Y;
	int chunkZ = z / CHUNK_Z;
	int xInChunk = x % CHUNK_X;
	int yInChunk = y % CHUNK_Y;
	int zInChunk = z % CHUNK_Z;
	if (xInChunk >= CHUNK_X || yInChunk >= CHUNK_Y || zInChunk >= CHUNK_Z)
		return; // Error
	if (xInChunk < 0)
		xInChunk += CHUNK_X;
	if (zInChunk < 0)
		zInChunk += CHUNK_Z;
	Chunk* chunk = getChunk(chunkX, chunkY, chunkZ);
	if (chunk != nullptr)
		chunk->blocks[xInChunk + zInChunk * CHUNK_X + yInChunk * CHUNK_X * CHUNK_Z].ID = id;
}

void Map::applyToAllChunks(std::function<void(Chunk*)> func)
{
	int	playerChunkY = static_cast<int>(Camera::getPlayerPosition().y) / CHUNK_Y;
	if (playerChunkY - RenderDistance >= MaxChunkHeight || playerChunkY + RenderDistance < 0)
		return;

	int size = RenderDistance * 2 + 1;

	int distance2 = (RenderDistance + 1) * (RenderDistance + 1);
	int maxY = playerChunkY + RenderDistance;
	int startY = playerChunkY - RenderDistance;
	if (startY < 0)
		startY = 0;
	if (maxY >= MaxChunkHeight)
		maxY = MaxChunkHeight - 1;
	for (int y = startY; y <= maxY; ++y)
	{
		for (int z = 0; z < size; ++z)
		{
			int distZ = RenderDistance - z;
			int distZ2 = distZ * distZ;
			for (int x = 0; x < size; ++x)
			{
				int distX = RenderDistance - x;
				int distX2 = distX * distX;
				if (distX2 + distZ2 > distance2)
					continue;
				func(_map[x + z * size + y * size * size]);
			}
		}
	}
}

void Map::_loadNewChunk(int x, int y, int z, int playerChunkX, int playerChunkY, int playerChunkZ)
{
	int chunkXpos = (x - RenderDistance + playerChunkX) * CHUNK_X;
	int chunkZpos = (z - RenderDistance + playerChunkZ) * CHUNK_Z;
	_tempBuffer[x + z * _size + y * _size * _size] = new Chunk(chunkXpos, y * CHUNK_Y, chunkZpos);
	for (int dy = -1; dy < 2; ++dy)
	{
		if (y + dy < 0 || y + dy >= MaxChunkHeight)
			continue;
		for (int dz = -1; dz < 2; ++dz)
		{
			if (z + dz < 0 || z + dz >= _size)
				continue;
			for (int dx = -1; dx < 2; ++dx)
			{
				if (x + dx < 0 || x + dx >= _size)
					continue;
				int pos = x + dx + (z + dz) * _size + (y + dy) * _size * _size;
				if (_tempBuffer[pos])
					_tempBuffer[pos]->updateNeighbors();
			}
		}
	}
}

void Map::updateMap()
{
	const glm::vec3& playerPos = Camera::getPlayerPosition();
	int	playerChunkY = static_cast<int>(playerPos.y) / CHUNK_Y;
	int	playerChunkX = static_cast<int>(playerPos.x) / CHUNK_X;
	int	playerChunkZ = static_cast<int>(playerPos.z) / CHUNK_Z;

	int startY = playerChunkY - RenderDistance;
	if (startY < 0)
		startY = 0;
	int maxY = playerChunkY + RenderDistance;
	if (maxY >= MaxChunkHeight)
		maxY = MaxChunkHeight - 1;

	int oldStartY = middleY - RenderDistance;
	if (oldStartY < 0)
		oldStartY = 0;
	int oldMaxY = middleY + RenderDistance;
	if (oldMaxY >= MaxChunkHeight)
		oldMaxY = MaxChunkHeight - 1;

	if (startY == oldStartY && maxY == oldMaxY)
		middleY = playerChunkY;

	if (playerChunkY - RenderDistance > MaxChunkHeight || \
		playerChunkX == middleX && playerChunkY == middleY && playerChunkZ == middleZ)
		return;

	memset(_tempBuffer, 0, _dataSize);

	int dx = playerChunkX - middleX;
	int dy = playerChunkY - middleY;
	int dz = playerChunkZ - middleZ;

	if (dx > _size || dx < -_size || \
		dy > _size || dy < -_size || \
		dz > _size || dz < -_size)
	{
		for (size_t i = 0; i < _dataSize / sizeof(Chunk*); ++i)
		{
			delete _map[i];
			_map[i] = nullptr;
		}
		_loadMap();
		return;
	}

	int distance2 = (RenderDistance + 1) * (RenderDistance + 1);

	for (int y = 0; y < MaxChunkHeight; ++y)
	{
		int distY = playerChunkY - y;
		bool includeY = distY <= RenderDistance && -distY <= RenderDistance;
		for (int z = 0; z < _size; ++z)
		{
			int distZ = RenderDistance - z + dz;
			int distZ2 = distZ * distZ;
			for (int x = 0; x < _size; ++x)
			{
				if (_map[x + z * _size + y * _size * _size] == nullptr)
					continue;
				int distX = RenderDistance - x + dx;
				int distX2 = distX * distX;
				if (!includeY || distX2 + distZ2 > distance2 || x - dx < 0 || z - dz < 0 || x - dx >= _size || z - dz >= _size)
				{
					delete _map[x + z * _size + y * _size * _size];
					_map[x + z * _size + y * _size * _size] = nullptr;
				}
				else
					_tempBuffer[x - dx + (z - dz) * _size + y * _size * _size] = _map[x + z * _size + y * _size * _size];
			}
		}
	}
	
	for (int y = startY; y <= maxY; ++y)
	{
		for (int z = 0; z < _size; ++z)
		{
			int distZ = RenderDistance - z;
			int distZ2 = distZ * distZ;
			for (int x = 0; x < _size; ++x)
			{
				int distX = RenderDistance - x;
				int distX2 = distX * distX;
				if (distX2 + distZ2 > distance2 || _tempBuffer[x + z * _size + y * _size * _size] != nullptr)
					continue;

				_loadNewChunk(x, y, z, playerChunkX, playerChunkY, playerChunkZ);
			}
		}
	}

	Chunk** temp = _map;
	_map = _tempBuffer;
	_tempBuffer = temp;

	middleX = playerChunkX;
	middleY = playerChunkY;
	middleZ = playerChunkZ;
}
