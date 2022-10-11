#include "TextureAtlas.hpp"
#include "json/json.hpp"
#include <fstream>
using json = nlohmann::json;

TextureAtlas::TextureAtlas()
{
	std::fstream blocksStream("res/blocks.json");
	json blocks = json::parse(blocksStream);

	_texture.createEmptyTexture(512, 512);

	int width = 0;
	int height = 0;
	int bpp = 0;

	unsigned char* buffer = 0;

	auto it = blocks.begin();

	stbi_set_flip_vertically_on_load(1);
	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			if (it == blocks.end())
				break;
			std::string path = "res/textures/" + it->get<std::string>() + ".png";
			buffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
			_texture.setSubImage(x * 32, (15 - y) * 32, width, height, buffer);
			if (buffer)
				stbi_image_free(buffer);
			++it;
		}
		if (it == blocks.end())
			break;
	}
	_texture.generateMipmap();
}

TextureAtlas::~TextureAtlas()
{}

void	TextureAtlas::bind(uint slot) const
{
	_texture.bind();
}

void	TextureAtlas::unbind() const
{
	_texture.unbind();
}
