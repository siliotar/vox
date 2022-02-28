#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(const void* data, uint size, uint usage)
{
	glGenBuffers(1, &_rendererId);
	glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &_rendererId);
}

void	VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
}

void	VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
#include "Profiler.hpp"
void	VertexBuffer::subData(const void* data, uint size) const
{
	InstrumentationTimer timer(__FUNCSIG__);
	bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
