#pragma once

typedef unsigned int uint;

#include <string>
#include "Window.hpp"
#include "stb_image.h"

class Texture
{
private:
	uint	_rendererId;
	int		_width;
	int		_height;
	int		_bpp;

	void _setParametri();
public:
	Texture();
	Texture(const std::string& path);
	~Texture();

	Texture(Texture& other) = delete;
	void operator=(const Texture& other) = delete;

	void	bind(uint slot = 0) const;
	void	unbind() const;

	inline int	getWidth() { return _width; }
	inline int	getHeight() { return _height; }

	void createEmptyTexture(int width, int height);
	void setSubImage(int xOffset, int yOffset, int width, int height, unsigned char* data);
	void generateMipmap();
};
