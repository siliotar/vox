#include "BlocksInfo.hpp"
#include "json/json.hpp"
using json = nlohmann::json;

#include <fstream>

BlocksInfo* BlocksInfo::_blocksInfo = nullptr;

int BlocksInfo::_addTexture(const std::string& texture)
{
	size_t size = _textures.size();
	for (size_t i = 0; i < size; ++i)
		if (_textures[i] == texture)
			return i;
	_textures.push_back(texture);
	return _textures.size() - 1;
}

BlocksInfo::BlocksInfo()
{
	std::fstream blocksStream("res/blocks.json");
	json blocks = json::parse(blocksStream);

	for (auto& block : blocks)
	{
		std::fstream currentBlockStream("res/models/" + block.get<std::string>() + ".json");
		json currentBlockData = json::parse(currentBlockStream);

		BlockInfo currentBlock;
		bool all = false;

		for (auto& face : currentBlockData["faces"].items())
		{
			const std::string& texture = face.value().get<std::string>();
			int position;
			if (texture[0] == '#')
				position = _addTexture(currentBlockData["textures"][texture.substr(1)].get<std::string>());
			else
				position = _addTexture(texture);
			if (face.key() == "all")
			{
				_blocks.emplace_back(position);
				all = true;
				break;
			}
			const std::string& key = face.key();
			if (key == "down")
				currentBlock.down = position;
			else if (key == "up")
				currentBlock.up = position;
			else if (key == "north")
				currentBlock.north = position;
			else if (key == "south")
				currentBlock.south = position;
			else if (key == "west")
				currentBlock.west = position;
			else if (key == "east")
				currentBlock.east = position;
		}

		if (!all)
			_blocks.push_back(currentBlock);
	}
}

BlocksInfo::~BlocksInfo()
{

}

void BlocksInfo::init()
{
	if (_blocksInfo == nullptr)
		_blocksInfo = new BlocksInfo();
}

void BlocksInfo::shutdown()
{
	delete _blocksInfo;
}

const std::vector<std::string>& BlocksInfo::getTextures()
{
	return _blocksInfo->_textures;
}

const BlockInfo& BlocksInfo::getBlockInfo(size_t id)
{
	return _blocksInfo->_blocks[id];
}
