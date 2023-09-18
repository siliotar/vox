#pragma once

#include "Texture.hpp"

class TextureAtlas
{
private:
	Texture _texture;
public:
	TextureAtlas();
	~TextureAtlas();

	void	bind(uint slot = 0) const;
	void	unbind() const;
};
