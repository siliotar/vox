#pragma once

typedef unsigned int uint;

#include "OpenGL.hpp"

class IndexBuffer
{
private:
	uint	_rendererId;
	uint	_count;
public:
	IndexBuffer(const uint* data, uint count, uint usage);
	~IndexBuffer();

	IndexBuffer(IndexBuffer& other) = delete;
	void operator=(const IndexBuffer& other) = delete;

	void	bind() const;
	void	unbind() const;

	void	subData(const void* data, uint size) const;

	inline uint	getCount() const { return _count; }
};
