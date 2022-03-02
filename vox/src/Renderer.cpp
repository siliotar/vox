#include "Renderer.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.hpp"
#include "Camera.hpp"

Renderer* Renderer::_renderer = nullptr;

Renderer::Renderer()
	: _va(), _vbLayout(),
	_ib(nullptr, MaxIndexCount * sizeof(GLuint), GL_STATIC_DRAW),
	_shader("res/shaders/Basic.vert", "res/shaders/Basic.frag"),
	_textureAtlas("res/textures/atlas.png"), _model(1.0f), _indexCount(0)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_vbLayout.push<float>(1);

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

void	Renderer::drawMap(Map& map)
{
	//int	playerChunkX = static_cast<int>(Camera::getPlayerPosition().x) / CHUNK_X;
	//int	playerChunkZ = static_cast<int>(Camera::getPlayerPosition().z) / CHUNK_Z;
	int	playerChunkX = 0;
	int	playerChunkZ = 0;
	_renderer->_va.bind();
	_renderer->_ib.bind();
	glm::mat4 mvp = Camera::getProjection() * Camera::getView() * _renderer->_model;
	_renderer->_shader.bind();
	_renderer->_shader.setUniformMatrix4f("MVP", mvp);
	for (int z = playerChunkZ - RenderDistance; z <= playerChunkZ + RenderDistance; ++z)
	{
		for (int x = playerChunkX - RenderDistance; x <= playerChunkX + RenderDistance; ++x)
		{
			const Chunk*	left = map.getChunk(x + 1, z);
			const Chunk*	right = map.getChunk(x - 1, z);
			const Chunk*	back = map.getChunk(x, z + 1);
			const Chunk*	front = map.getChunk(x, z - 1);
			map.getChunk(x, z)->calculateMesh(left, right, back, front);
			map.getChunk(x, z)->draw(_renderer->_va, _renderer->_vbLayout, _renderer->_shader);
		}
	}
}
