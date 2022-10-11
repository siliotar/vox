#include "Texture.hpp"
#include "Settings.hpp"

Texture::Texture()
	: _rendererId(0), _width(0), _height(0), _bpp(0)
{}

void Texture::_setParametri()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, MipmapLevel);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

Texture::Texture(const std::string& path)
	: _rendererId(0), _width(0), _height(0), _bpp(0)
{
	stbi_set_flip_vertically_on_load(1);
	unsigned char* buffer = stbi_load(path.c_str(), &_width, &_height, &_bpp, 4);

	glGenTextures(1, &_rendererId);
	glBindTexture(GL_TEXTURE_2D, _rendererId);

	_setParametri();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (buffer)
		stbi_image_free(buffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &_rendererId);
}

void	Texture::bind(uint slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _rendererId);
}

void	Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::createEmptyTexture(int width, int height)
{
	_width = width;
	_height = height;


	glGenTextures(1, &_rendererId);
	glBindTexture(GL_TEXTURE_2D, _rendererId);

	_setParametri();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setSubImage(int xOffset, int yOffset, int width, int height, unsigned char* data)
{
	bind();
	glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	unbind();
}

void Texture::generateMipmap()
{
	bind();
	glGenerateMipmap(GL_TEXTURE_2D);
	unbind();
}
