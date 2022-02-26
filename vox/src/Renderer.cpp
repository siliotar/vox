#include "Renderer.hpp"

Renderer* Renderer::_renderer = nullptr;
const float Renderer::TextureSize = 32.0f / 512.0f;

Renderer::Renderer()
	: _va(), _vb(nullptr, MaxVertexCount * sizeof(Vertex), GL_DYNAMIC_DRAW), _vbLayout(),
	_ib(nullptr, MaxIndexCount * sizeof(GLuint), GL_DYNAMIC_DRAW),
	_shader("res/shaders/Basic.vert", "res/shaders/Basic.frag"),
	_textureAtlas("res/textures/atlas.png"), _model(1.0f),
	_vertexCount(0), _indexCount(0)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_vbLayout.push<float>(3);
	_vbLayout.push<float>(2);
	_vbLayout.push<float>(1);
	_va.addBuffer(_vb, _vbLayout);

	_rectVertexBuffer = new Vertex[MaxVertexCount];
	_rectIndexBuffer = new GLuint[MaxIndexCount];

	_shader.bind();
	_textureAtlas.bind();
	_shader.setUniform1i("u_Texture", 0);
	_shader.setUniform2f("u_Texture", TextureAtlasX, TextureAtlasY);

	//_view = glm::translate(_view, glm::vec3(0.0f, 0.0f, -2.0f));
	//_view = glm::rotate(_view, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glClearColor(0.2f, 0.3f, 0.6f, 0.0f);
}

Renderer::~Renderer()
{
	delete[] _rectVertexBuffer;
	delete[] _rectIndexBuffer;
}

void	Renderer::init()
{
	if (_renderer != nullptr)
		return;

	_renderer = new Renderer();
}

void	Renderer::shutdown()
{
	if (_renderer == nullptr)
		return;

	delete _renderer;
	_renderer = nullptr;
}

void	Renderer::beginBatch()
{
	_renderer->_rectVertexBufferPtr = _renderer->_rectVertexBuffer;
	_renderer->_rectIndexBufferPtr = _renderer->_rectIndexBuffer;
	glm::mat4 mvp = Camera::getProjection() * Camera::getView() * _renderer->_model;
	_renderer->_shader.setUniformMatrix4f("MVP", mvp);
}

void	Renderer::endBatch()
{
	GLsizeiptr	vertexBufferSize = (uint8_t*)_renderer->_rectVertexBufferPtr - (uint8_t*)_renderer->_rectVertexBuffer;
	_renderer->_vb.subData(_renderer->_rectVertexBuffer, vertexBufferSize);

	GLsizeiptr	indexBufferSize = (uint8_t*)_renderer->_rectIndexBufferPtr - (uint8_t*)_renderer->_rectIndexBuffer;
	_renderer->_ib.subData(_renderer->_rectIndexBuffer, indexBufferSize);
}

void	Renderer::flush()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_renderer->_va.bind();
	_renderer->_ib.bind();

	glDrawElements(GL_TRIANGLES, _renderer->_indexCount, GL_UNSIGNED_INT, nullptr);

	_renderer->_vertexCount = 0;
	_renderer->_indexCount = 0;
}

void Renderer::drawTriangle()
{
	if (_renderer->_indexCount >= MaxIndexCount)
	{
		endBatch();
		flush();
		beginBatch();
	}
}

void Renderer::drawRectangle(const glm::vec3& leftDownPos, const glm::vec3& rightDownPos, \
							const glm::vec3& rightUpPos, const glm::vec3& leftUpPos, uint textureID)
{
	if (_renderer->_indexCount >= MaxIndexCount)
	{
		endBatch();
		flush();
		beginBatch();
	}

	float	textureLeft = (float)(textureID % TextureAtlasX) * TextureSize;
	float	textureDown = (float)(TextureAtlasY - 1 - textureID / TextureAtlasY) * TextureSize;

	_renderer->_rectVertexBufferPtr->position = leftDownPos;
	_renderer->_rectVertexBufferPtr->texCoords = { textureLeft, textureDown };
	_renderer->_rectVertexBufferPtr->texIndex = textureID;
	++(_renderer->_rectVertexBufferPtr);

	_renderer->_rectVertexBufferPtr->position = rightDownPos;
	_renderer->_rectVertexBufferPtr->texCoords = { textureLeft + TextureSize, textureDown };
	_renderer->_rectVertexBufferPtr->texIndex = textureID;
	++(_renderer->_rectVertexBufferPtr);

	_renderer->_rectVertexBufferPtr->position = rightUpPos;
	_renderer->_rectVertexBufferPtr->texCoords = { textureLeft + TextureSize, textureDown + TextureSize };
	_renderer->_rectVertexBufferPtr->texIndex = textureID;
	++(_renderer->_rectVertexBufferPtr);

	_renderer->_rectVertexBufferPtr->position = leftUpPos;
	_renderer->_rectVertexBufferPtr->texCoords = { textureLeft, textureDown + TextureSize };
	_renderer->_rectVertexBufferPtr->texIndex = textureID;
	++(_renderer->_rectVertexBufferPtr);

	_renderer->_rectIndexBufferPtr[0] = _renderer->_vertexCount + 0;
	_renderer->_rectIndexBufferPtr[1] = _renderer->_vertexCount + 1;
	_renderer->_rectIndexBufferPtr[2] = _renderer->_vertexCount + 2;
	_renderer->_rectIndexBufferPtr[3] = _renderer->_vertexCount + 2;
	_renderer->_rectIndexBufferPtr[4] = _renderer->_vertexCount + 3;
	_renderer->_rectIndexBufferPtr[5] = _renderer->_vertexCount + 0;

	_renderer->_vertexCount += 4;
	_renderer->_indexCount += 6;
	_renderer->_rectIndexBufferPtr += 6;
}
