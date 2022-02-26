#include "VertexArray.hpp"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &_rendererId);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &_rendererId);
}

void	VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	uint offset = 0;
	for (uint i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void	VertexArray::bind() const
{
	glBindVertexArray(_rendererId);
}

void	VertexArray::unbind() const
{
	glBindVertexArray(0);
}
