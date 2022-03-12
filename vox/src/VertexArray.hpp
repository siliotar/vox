#pragma once

#include "OpenGL.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
private:
	uint	_rendererId;
public:
	VertexArray();
	~VertexArray();

	VertexArray(VertexArray& other) = delete;
	void operator=(const VertexArray& other) = delete;

	void	addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void	addBuffer(const VertexBuffer& vb, const VertexBuffer& instancevb, const VertexBufferLayout& layout);

	void	bind() const;
	void	unbind() const;
};
