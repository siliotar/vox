#include "Renderer.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Settings.hpp"

Renderer* Renderer::_renderer = nullptr;

Renderer::Renderer()
	: _va(), _voxelvbLayout(),
	_ib(nullptr, MaxIndexCount * sizeof(GLuint), GL_STATIC_DRAW),
	_voxelShader("res/shaders/Basic.vert", "res/shaders/Basic.frag"),
	_voxelTextureAtlas("res/textures/atlas.png"), _model(1.0f), _indexCount(0)
{
	_voxelvbLayout.push<uint32_t>(1);

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

	_voxelShader.bind();
	_voxelTextureAtlas.bind();
	_voxelShader.setUniform1i("u_Texture", 0);
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
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int	playerChunkX = static_cast<int>(Camera::getPlayerPosition().x) / CHUNK_X;
	int	playerChunkY = static_cast<int>(Camera::getPlayerPosition().y) / CHUNK_Y;
	int	playerChunkZ = static_cast<int>(Camera::getPlayerPosition().z) / CHUNK_Z;
	_renderer->_va.bind();
	_renderer->_ib.bind();
	glm::mat4 mvp = Camera::getProjection() * Camera::getView() * _renderer->_model;
	_renderer->_voxelShader.bind();
	_renderer->_voxelShader.setUniformMatrix4f("MVP", mvp);
	_renderer->_voxelTextureAtlas.bind();
	float distance2 = (RenderDistance + 1) * (RenderDistance + 1);
	for (int y = playerChunkY - RenderDistance; y <= playerChunkY + RenderDistance; ++y)
	{
		for (int z = playerChunkZ - RenderDistance; z <= playerChunkZ + RenderDistance; ++z)
		{
			for (int x = playerChunkX - RenderDistance; x <= playerChunkX + RenderDistance; ++x)
			{
				if (y < 0 || y >= MaxChunkHeight)
					continue;
				float distX = (float)playerChunkX - x;
				float distZ = (float)playerChunkZ - z;
				if (distX * distX + distZ * distZ > distance2)
					continue;
				Chunk* tempChunk = map.getChunk(x, y, z);
				tempChunk->calculateGreedyMesh(map);
				tempChunk->draw(_renderer->_va, _renderer->_voxelvbLayout, _renderer->_voxelShader);
			}
		}
	}
}
