#pragma once

typedef unsigned int uint;

#include "OpenGL.hpp"

class VertexBuffer
{
private:
	uint	_rendererId;
public:
	VertexBuffer(const void* data, uint size, uint usage);
	~VertexBuffer();

	VertexBuffer(VertexBuffer& other) = delete;
	void operator=(const VertexBuffer& other) = delete;

	void	bind() const;
	void	unbind() const;

	void	subData(const void* data, uint size) const;
};
