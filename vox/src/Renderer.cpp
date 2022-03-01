#include "Renderer.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.hpp"
#include "Camera.hpp"

Renderer* Renderer::_renderer = nullptr;

Renderer::Renderer()
	: _va(), _vb(nullptr, MaxVertexCount * sizeof(Vertex), GL_DYNAMIC_DRAW), _vbLayout(),
	_ib(nullptr, MaxIndexCount * sizeof(GLuint), GL_DYNAMIC_DRAW),
	_shader("res/shaders/Basic.vert", "res/shaders/Basic.frag"),
	_textureAtlas("res/textures/atlas.png"), _model(1.0f), _indexCount(0)
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

	_renderer->_indexCount = 0;
}

void	Renderer::drawMap(Map& map)
{
	//int	playerChunkX = static_cast<int>(Camera::getPlayerPosition().x) / CHUNK_X;
	//int	playerChunkZ = static_cast<int>(Camera::getPlayerPosition().z) / CHUNK_Z;
	int	playerChunkX = 0;
	int	playerChunkZ = 0;
	for (int z = playerChunkZ - RenderDistance; z <= playerChunkZ + RenderDistance; ++z)
	{
		for (int x = playerChunkX - RenderDistance; x <= playerChunkX + RenderDistance; ++x)
		{
			const Chunk*	left = map.getChunk(x + 1, z);
			const Chunk*	right = map.getChunk(x - 1, z);
			const Chunk*	back = map.getChunk(x, z + 1);
			const Chunk*	front = map.getChunk(x, z - 1);
			map.getChunk(x, z)->calculateMesh(left, right, back, front);
			std::vector<Vertex>& mesh = map.getChunk(x, z)->mesh;
			size_t size = mesh.size();
			Vertex* data = mesh.data();
			for (size_t k = 0; k < size; k += 4)
			{
				if (_renderer->_indexCount >= MaxIndexCount)
				{
					endBatch();
					flush();
					beginBatch();
				}

				memcpy(_renderer->_rectVertexBufferPtr, &data[k], 4 * sizeof(Vertex));

				_renderer->_rectVertexBufferPtr += 4;
				_renderer->_indexCount += 6;
			}
		}
	}
}
