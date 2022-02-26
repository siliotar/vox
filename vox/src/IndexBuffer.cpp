#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const uint* data, uint count, uint usage)
	: _count(count)
{
	glGenBuffers(1, &_rendererId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, usage);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &_rendererId);
}

void	IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
}

void	IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void	IndexBuffer::subData(const void* data, uint size) const
{
	bind();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}
