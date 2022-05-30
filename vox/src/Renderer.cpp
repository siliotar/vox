#include "Renderer.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Settings.hpp"

Renderer* Renderer::_renderer = nullptr;

Renderer::Renderer()
	: _va(), _voxelvbLayout(),
	_ib(nullptr, 6 * sizeof(GLuint), GL_STATIC_DRAW),
	_voxelShader("res/shaders/Basic.vert", "res/shaders/Basic.frag"),
	_voxelTextureAtlas("res/textures/atlas.png")
{
	_voxelvbLayout.push<uint32_t>(2, 1);

	GLuint rectIndexBuffer[6] = {
		0, 1, 2,
		2, 3, 0
	};

	_ib.subData(rectIndexBuffer, 6 * sizeof(GLuint));

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
	glm::mat4 mvp = Camera::getProjection() * Camera::getView();
	_renderer->_voxelShader.bind();
	_renderer->_voxelShader.setUniformMatrix4f(_renderer->_MVPUniformName, mvp);
	_renderer->_voxelTextureAtlas.bind();
	int distance2 = (RenderDistance + 1) * (RenderDistance + 1);
	int maxX = playerChunkX + RenderDistance;
	int maxY = playerChunkY + RenderDistance;
	int maxZ = playerChunkZ + RenderDistance;
	int startY = playerChunkY - RenderDistance;
	if (startY < 0)
		startY = 0;
	if (maxY >= MaxChunkHeight)
		maxY = MaxChunkHeight - 1;
	for (int y = startY; y <= maxY; ++y)
	{
		for (int z = playerChunkZ - RenderDistance; z <= maxZ; ++z)
		{
			int distZ = playerChunkZ - z;
			int distZ2 = distZ * distZ;
			for (int x = playerChunkX - RenderDistance; x <= maxX; ++x)
			{
				int distX = playerChunkX - x;
				int distX2 = distX * distX;
				if (distX2 + distZ2 > distance2)
					continue;
				Chunk* tempChunk = map.getChunk(x, y, z);
				tempChunk->calculateGreedyMesh(map);
				tempChunk->draw(_renderer->_va, _renderer->_voxelvbLayout, _renderer->_voxelShader);
			}
		}
	}
}
