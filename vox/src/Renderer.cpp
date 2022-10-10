#include "Renderer.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Settings.hpp"
#include <future>
#include "Frustum.hpp"

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

static void calculateMesh(Chunk* chunk, Map* map)
{
	chunk->calculateGreedyMesh(*map);
}

static void chechIsCulled(std::vector<Chunk*> order, Frustum* camFrustum)
{
	for (auto chunk : order)
		chunk->checkIsCulled(*camFrustum);
}

void	Renderer::drawMap(Map& map)
{
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_renderer->_va.bind();
	_renderer->_ib.bind();
	glm::mat4 mvp = Camera::getProjection() * Camera::getView();
	_renderer->_voxelShader.bind();
	_renderer->_voxelShader.setUniformMatrix4f(_renderer->_MVPUniformName, mvp);
	_renderer->_voxelTextureAtlas.bind();

	Frustum camFrustum = createFrustumFromCamera();
	std::vector<Chunk*> orderToCheckCulling;
	orderToCheckCulling.reserve(1024);
	map.updateMap();
	{
		std::vector<std::future<void>> futures;
		map.applyToAllChunks([&map, &futures, &orderToCheckCulling, &camFrustum](Chunk* chunk)
			{
				if (chunk->modified)
					futures.push_back(std::async(std::launch::async, calculateMesh, chunk, &map));
				orderToCheckCulling.push_back(chunk);
				if (orderToCheckCulling.size() == 1024)
				{
					futures.push_back(std::async(std::launch::async, chechIsCulled, orderToCheckCulling, &camFrustum));
					orderToCheckCulling.clear();
				}
			});
		if (orderToCheckCulling.size() > 0)
			chechIsCulled(orderToCheckCulling, &camFrustum);
	}
	map.applyToAllChunks([=](Chunk* chunk)
		{
			if (!chunk->isCulled)
				chunk->draw(_renderer->_va, _renderer->_voxelvbLayout, _renderer->_voxelShader);
		});
}
