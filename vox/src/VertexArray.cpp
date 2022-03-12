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
		if (element.type == GL_UNSIGNED_INT || element.type == GL_UNSIGNED_BYTE || element.type == GL_INT)
			glVertexAttribIPointer(i, element.count, element.type, layout.getStride(), (const void*)offset);
		else
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		if (element.divisor != 0)
			glVertexAttribDivisor(i, element.divisor);
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void	VertexArray::addBuffer(const VertexBuffer& vb, const VertexBuffer& instancevb, const VertexBufferLayout& layout)
{
	bind();
	const auto& elements = layout.getElements();
	uint vbOffset = 0;
	uint instanceOffset = 0;
	uint offset;
	for (uint i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		if (element.divisor == 0)
		{
			vb.bind();
			offset = vbOffset;
		}
		else
		{
			instancevb.bind();
			offset = instanceOffset;
		}
		glEnableVertexAttribArray(i);
		if (element.type == GL_UNSIGNED_INT || element.type == GL_UNSIGNED_BYTE || element.type == GL_INT)
			glVertexAttribIPointer(i, element.count, element.type, layout.getStride(), (const void*)offset);
		else
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		if (element.divisor == 0)
			vbOffset += element.count * VertexBufferElement::getSizeOfType(element.type);
		else
		{
			glVertexAttribDivisor(i, element.divisor);
			instanceOffset += element.count * VertexBufferElement::getSizeOfType(element.type);
		}
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
