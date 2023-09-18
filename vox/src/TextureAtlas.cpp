#include "TextureAtlas.hpp"
#include "BlocksInfo.hpp"

TextureAtlas::TextureAtlas()
{
	_texture.createEmptyTexture(512, 512);

	int width = 0;
	int height = 0;
	int bpp = 0;

	unsigned char* buffer = 0;

	const std::vector<std::string>& textures = BlocksInfo::getTextures();
	auto it = textures.cbegin();

	stbi_set_flip_vertically_on_load(1);
	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			if (it == textures.cend())
				break;
			std::string path = "res/textures/" + *it + ".png";
			buffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
			_texture.setSubImage(x * 32, (15 - y) * 32, width, height, buffer);
			if (buffer)
				stbi_image_free(buffer);
			++it;
		}
		if (it == textures.cend())
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
