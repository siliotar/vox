#pragma once

#include "OpenGL.hpp"
#include "glm/glm.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "Chunk.hpp"
#include "Map.hpp"
#include "Vertex.hpp"

class Renderer
{
private:
	static Renderer* _renderer;
	VertexArray _va;
	VertexBufferLayout _vbLayout;
	IndexBuffer _ib;
	GLuint* _rectIndexBuffer = nullptr;
	Shader _shader;
	Texture _textureAtlas;
	uint _indexCount;

	glm::mat4	_model;

	static const size_t	MaxRectCount = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 / 2;
	static const size_t	MaxIndexCount = MaxRectCount * 6;

	Renderer();
	~Renderer();
public:
	Renderer(Renderer& other) = delete;
	void operator=(const Renderer& other) = delete;

	static void	init();
	static void	shutdown();

	static void drawMap(Map& map);
};
