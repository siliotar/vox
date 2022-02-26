#pragma once

#include "OpenGL.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Camera.hpp"

struct Vertex
{
	glm::vec3	position;
	glm::vec2	texCoords;
	float		texIndex;
};

class Renderer
{
private:
	static Renderer*	_renderer;
	VertexArray	_va;
	VertexBuffer	_vb;
	VertexBufferLayout	_vbLayout;
	IndexBuffer	_ib;
	Vertex* _rectVertexBuffer = nullptr;
	Vertex* _rectVertexBufferPtr = nullptr;
	GLuint* _rectIndexBuffer = nullptr;
	GLuint* _rectIndexBufferPtr = nullptr;
	Shader	_shader;
	Texture	_textureAtlas;
	uint	_vertexCount;
	uint	_indexCount;

	glm::mat4	_model;

	static const size_t	MaxRectCount = 10000;
	static const size_t	MaxVertexCount = MaxRectCount * 4;
	static const size_t	MaxIndexCount = MaxRectCount * 6;
	static const size_t	TextureAtlasX = 16;
	static const size_t	TextureAtlasY = 16;
	static const float	TextureSize;

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

	static void drawTriangle();
	static void drawRectangle(const glm::vec3& leftDownPos, const glm::vec3& rightDownPos, \
							const glm::vec3& rightUpPos, const glm::vec3& leftUpPos, uint textureID);
};
