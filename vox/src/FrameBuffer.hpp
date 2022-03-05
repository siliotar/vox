#pragma once

#include <initializer_list>
#include <vector>
#include "OpenGL.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

enum class FrameBufferTextureFormat
{
	None = 0,

	// Color
	RGBA8 = GL_RGBA8,
	RED_INTEGER = GL_RED_INTEGER,

	// Depth/stencil
	DEPTH24STENCIL8 = GL_DEPTH24_STENCIL8,

	// Defaults
	Depth = DEPTH24STENCIL8
};

struct FrameBufferTextureSpecification
{
	FrameBufferTextureSpecification() = default;
	FrameBufferTextureSpecification(FrameBufferTextureFormat format)
		: TextureFormat(format) {}

	FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
};

struct FrameBufferAttachmentSpecification
{
	FrameBufferAttachmentSpecification() = default;
	FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
		: Attachments(attachments) {}

	std::vector<FrameBufferTextureSpecification> Attachments;
};

struct FrameBufferSpecification
{
	uint32_t Width = 0, Height = 0;
	FrameBufferAttachmentSpecification Attachments;
	uint32_t Samples = 1;

	bool SwapChainTarget = false;
};

class FrameBuffer
{
private:
	uint32_t _rendererID = 0;
	FrameBufferSpecification _specification;

	std::vector<FrameBufferTextureSpecification> _colorAttachmentSpecifications;
	FrameBufferTextureSpecification _depthAttachmentSpecification = FrameBufferTextureFormat::None;

	std::vector<uint32_t> _colorAttachments;
	uint32_t _depthAttachment = 0;

	Shader	_shader;
public:
	FrameBuffer(const FrameBufferSpecification& spec);
	~FrameBuffer();

	void Invalidate();

	void bind();
	void unbind();

	void resize(uint32_t width, uint32_t height);
	int readPixel(uint32_t attachmentIndex, int x, int y);

	void clearAttachment(uint32_t attachmentIndex, int value);

	uint32_t getColorAttachmentRendererID(uint32_t index = 0) const;

	const FrameBufferSpecification& getSpecification() const;

	void draw();
};
