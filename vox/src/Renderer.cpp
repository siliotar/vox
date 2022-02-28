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
	_va.addBuffer(_vb, _vbLayout);

	_rectVertexBuffer = new Vertex[MaxVertexCount];
	_rectIndexBuffer = new GLuint[MaxIndexCount];

	size_t offset = 0;
	for (size_t i = 0; i < MaxIndexCount;)
	{
		_rectIndexBuffer[i + 0] = 0 + offset;
		_rectIndexBuffer[i + 1] = 1 + offset;
		_rectIndexBuffer[i + 2] = 2 + offset;
		_rectIndexBuffer[i + 3] = 2 + offset;
		_rectIndexBuffer[i + 4] = 3 + offset;
		_rectIndexBuffer[i + 5] = 0 + offset;
		i += 6;
		offset += 4;
	}

	_ib.subData(_rectIndexBuffer, MaxIndexCount * sizeof(GLuint));
	delete[] _rectIndexBuffer;

	_shader.bind();
	_textureAtlas.bind();
	_shader.setUniform1i("u_Texture", 0);

	glClearColor(0.2f, 0.3f, 0.6f, 0.0f);
}

Renderer::~Renderer()
{
	delete[] _rectVertexBuffer;
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
	glm::mat4 mvp = Camera::getProjection() * Camera::getView() * _renderer->_model;
	_renderer->_shader.setUniformMatrix4f("MVP", mvp);
}

void	Renderer::endBatch()
{
	GLsizeiptr	vertexBufferSize = (uint8_t*)_renderer->_rectVertexBufferPtr - (uint8_t*)_renderer->_rectVertexBuffer;
	_renderer->_vb.subData(_renderer->_rectVertexBuffer, vertexBufferSize);
}

void	Renderer::flush()
{
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
	++(_renderer->_rectVertexBufferPtr);

	_renderer->_rectVertexBufferPtr->position = rightDownPos;
	_renderer->_rectVertexBufferPtr->texCoords = { textureLeft + TextureSize, textureDown };
	++(_renderer->_rectVertexBufferPtr);

	_renderer->_rectVertexBufferPtr->position = rightUpPos;
	_renderer->_rectVertexBufferPtr->texCoords = { textureLeft + TextureSize, textureDown + TextureSize };
	++(_renderer->_rectVertexBufferPtr);

	_renderer->_rectVertexBufferPtr->position = leftUpPos;
	_renderer->_rectVertexBufferPtr->texCoords = { textureLeft, textureDown + TextureSize };
	++(_renderer->_rectVertexBufferPtr);

	_renderer->_vertexCount += 4;
	_renderer->_indexCount += 6;
}

void	Renderer::drawChunk(const Chunk& chunk, const Chunk* left, const Chunk* right, const Chunk* back, const Chunk* front)
{
	for (size_t y = 0; y < CHUNK_Y; ++y)
	{
		for (size_t z = 0; z < CHUNK_Z; z++)
		{
			for (size_t x = 0; x < CHUNK_X; x++)
			{
				int	posX = x + chunk.x;
				int	posY = y;
				int	posZ = z + chunk.z;
				if (chunk.blocks[x + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)
					continue;
				if (y == 0 || chunk.blocks[x + z * CHUNK_X + (y - 1) * CHUNK_Z * CHUNK_X].ID == 0)
					drawRectangle(
						{ posX - 0.5f, posY - 0.5f, posZ - 0.5f },
						{ posX + 0.5f, posY - 0.5f, posZ - 0.5f },
						{ posX + 0.5f, posY - 0.5f, posZ + 0.5f },
						{ posX - 0.5f, posY - 0.5f, posZ + 0.5f },
						chunk.blocks[x + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].Texture);
				if (y == (CHUNK_Y - 1) || chunk.blocks[x + z * CHUNK_X + (y + 1) * CHUNK_Z * CHUNK_X].ID == 0)
					drawRectangle(
						{ posX - 0.5f, posY + 0.5f, posZ + 0.5f },
						{ posX + 0.5f, posY + 0.5f, posZ + 0.5f },
						{ posX + 0.5f, posY + 0.5f, posZ - 0.5f },
						{ posX - 0.5f, posY + 0.5f, posZ - 0.5f },
						chunk.blocks[x + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].Texture);
				if ((z == 0 && (!front || front->blocks[x + (CHUNK_Z - 1) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (z != 0 && chunk.blocks[x + (z - 1) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
					drawRectangle(
						{ posX + 0.5f, posY - 0.5f, posZ - 0.5f },
						{ posX - 0.5f, posY - 0.5f, posZ - 0.5f },
						{ posX - 0.5f, posY + 0.5f, posZ - 0.5f },
						{ posX + 0.5f, posY + 0.5f, posZ - 0.5f },
						chunk.blocks[x + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].Texture);
				if ((z == (CHUNK_Z - 1) && (!back || back->blocks[x + (0) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (z != (CHUNK_Z - 1) && chunk.blocks[x + (z + 1) * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
					drawRectangle(
						{ posX - 0.5f, posY - 0.5f, posZ + 0.5f },
						{ posX + 0.5f, posY - 0.5f, posZ + 0.5f },
						{ posX + 0.5f, posY + 0.5f, posZ + 0.5f },
						{ posX - 0.5f, posY + 0.5f, posZ + 0.5f },
						chunk.blocks[x + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].Texture);
				if ((x == 0 && (!left || left->blocks[(CHUNK_X - 1) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (x != 0 && chunk.blocks[(x - 1) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
					drawRectangle(
						{ posX - 0.5f, posY - 0.5f, posZ - 0.5f },
						{ posX - 0.5f, posY - 0.5f, posZ + 0.5f },
						{ posX - 0.5f, posY + 0.5f, posZ + 0.5f },
						{ posX - 0.5f, posY + 0.5f, posZ - 0.5f },
						chunk.blocks[x + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].Texture);
				if ((x == (CHUNK_X - 1) && (!right || right->blocks[(0) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0)) \
					|| (x != (CHUNK_X - 1) && chunk.blocks[(x + 1) + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].ID == 0))
					drawRectangle(
						{ posX + 0.5f, posY - 0.5f, posZ + 0.5f },
						{ posX + 0.5f, posY - 0.5f, posZ - 0.5f },
						{ posX + 0.5f, posY + 0.5f, posZ - 0.5f },
						{ posX + 0.5f, posY + 0.5f, posZ + 0.5f },
						chunk.blocks[x + z * CHUNK_X + y * CHUNK_Z * CHUNK_X].Texture);
			}
		}
	}
}

void	Renderer::drawMap(const Map& map)
{
	size_t size = map.radius * 2 + 1;
	for (size_t i = 0; i < size; ++i)
	{
		for (size_t j = 0; j < size; ++j)
		{
			const Chunk*	left = j == size - 1 ? 0 : &(map.map[j + 1 + i * size]);
			const Chunk*	right = j == 0 ? 0 : &(map.map[j - 1 + i * size]);
			const Chunk*	back = i == 0 ? 0 : &(map.map[j + (i - 1) * size]);
			const Chunk*	front = i == size - 1 ? 0 : &(map.map[j + (i + 1) * size]);
			drawChunk(map.map[j + i * size], left, right, back, front);
		}
	}
}
