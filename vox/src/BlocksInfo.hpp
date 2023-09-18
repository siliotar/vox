#pragma once

#include <vector>
#include <string>

struct BlockInfo
{
	int down;
	int up;
	int north;
	int south;
	int west;
	int east;

	BlockInfo() : down(0), up(0), north(0), south(0), west(0), east(0) {}
	BlockInfo(int all) : down(all), up(all), north(all), south(all), west(all), east(all) {}
	BlockInfo(int down, int up, int north, int south, int west, int east)
		: down(down), up(up), north(north), south(south), west(west), east(east) {}
};

class BlocksInfo
{
private:
	static BlocksInfo* _blocksInfo;

	std::vector<BlockInfo> _blocks;
	std::vector<std::string> _textures;

	BlocksInfo();
	~BlocksInfo();

	int _addTexture(const std::string& texture);

public:
	static void init();
	static void	shutdown();

	static const std::vector<std::string>& getTextures();
	static const BlockInfo& getBlockInfo(size_t id);
};
