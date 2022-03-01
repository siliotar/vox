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
	VertexBuffer _vb;
	VertexBufferLayout _vbLayout;
	IndexBuffer _ib;
	Vertex* _rectVertexBuffer = nullptr;
	Vertex* _rectVertexBufferPtr = nullptr;
	GLuint* _rectIndexBuffer = nullptr;
	Shader _shader;
	Texture _textureAtlas;
	uint _indexCount;

	glm::mat4	_model;

	static const size_t	MaxRectCount = 10000;
	static const size_t	MaxVertexCount = MaxRectCount * 4;
	static const size_t	MaxIndexCount = MaxRectCount * 6;

	Renderer();
	~Renderer();
public:
	Renderer(Renderer& other) = delete;
	void operator=(const Renderer& other) = delete;

	static void	init();
	static void	shutdown();

	static void	beginBatch();
	static void	endBatch();
	static void	flush();

	static void drawMap(Map& map);
};
