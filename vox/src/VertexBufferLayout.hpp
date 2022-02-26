#pragma once

#include <vector>
#include "OpenGL.hpp"

struct VertexBufferElement
{
	uint	type;
	uint	count;
	unsigned char	normalized;

	static uint	getSizeOfType(uint type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		default:
			break;
		}
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement>	_elements;
	uint	_stride;
public:
	VertexBufferLayout()
		: _stride(0) {}
	~VertexBufferLayout() {}

	VertexBufferLayout(VertexBufferLayout& other) = delete;
	void operator=(const VertexBufferLayout& other) = delete;

	template <typename T>
	void	push(uint count)
	{
		static_assert(false);
	}

	template <>
	void	push<float>(uint count)
	{
		_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template <>
	void	push<uint>(uint count)
	{
		_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template <>
	void	push<unsigned char>(uint count)
	{
		_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& getElements() const { return _elements; }
	inline uint	getStride() const { return _stride; }
};
