#pragma once

#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "TextureAtlas.hpp"
#include "Map.hpp"

class Renderer
{
private:
	static Renderer* _renderer;
	VertexArray _va;
	VertexBufferLayout _voxelvbLayout;
	IndexBuffer _ib;
	Shader _voxelShader;
	TextureAtlas _voxelTextureAtlas;

	const std::string _MVPUniformName = "MVP";

	Renderer();
	~Renderer();
public:
	Renderer(Renderer& other) = delete;
	void operator=(const Renderer& other) = delete;

	static void	init();
	static void	shutdown();

	static void drawMap(Map& map);
};
