#include "FrameBuffer.hpp"
#include "VertexBuffer.hpp"

static const uint32_t MaxFrameBufferSize = 8192;

static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
{
	bool multisampled = samples > 1;
	if (multisampled)
	{
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, id, 0);
}

static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
{
	bool multisampled = samples > 1;
	if (multisampled)
	{
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
	}
	else
	{
		glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, id, 0);
}

FrameBuffer::FrameBuffer(const FrameBufferSpecification& spec)
	: _specification(spec), _shader("res/shaders/Framebuffer.vert", "res/shaders/Framebuffer.frag")
{
	for (auto format : _specification.Attachments.Attachments)
	{
		if (format.TextureFormat != FrameBufferTextureFormat::DEPTH24STENCIL8)
			_colorAttachmentSpecifications.emplace_back(format);
		else
			_depthAttachmentSpecification = format;
	}
	Invalidate();
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &_rendererID);
	glDeleteTextures(_colorAttachments.size(), _colorAttachments.data());
	glDeleteTextures(1, &_depthAttachment);
}

void FrameBuffer::Invalidate()
{
	if (_rendererID)
	{
		glDeleteFramebuffers(1, &_rendererID);
		glDeleteTextures(_colorAttachments.size(), _colorAttachments.data());
		glDeleteTextures(1, &_depthAttachment);

		_colorAttachments.clear();
		_depthAttachment = 0;
	}

	glCreateFramebuffers(1, &_rendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, _rendererID);
	
	GLenum target = _specification.Samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	if (_colorAttachmentSpecifications.size())
	{
		_colorAttachments.resize(_colorAttachmentSpecifications.size());
		glCreateTextures(target, _colorAttachments.size(), _colorAttachments.data());

		for (size_t i = 0; i < _colorAttachments.size(); ++i)
		{
			glBindTexture(target, _colorAttachments[i]);
			switch (_colorAttachmentSpecifications[i].TextureFormat)
			{
			case FrameBufferTextureFormat::RGBA8:
				AttachColorTexture(_colorAttachments[i], _specification.Samples, GL_RGBA8, GL_RGBA, _specification.Width, _specification.Height, i);
				break;
			case FrameBufferTextureFormat::RED_INTEGER:
				AttachColorTexture(_colorAttachments[i], _specification.Samples, GL_R32I, GL_RED_INTEGER, _specification.Width, _specification.Height, i);
				break;
			}
		}
	}

	if (_depthAttachmentSpecification.TextureFormat != FrameBufferTextureFormat::None)
	{
		glCreateTextures(target, 1, &_depthAttachment);
		glBindTexture(target, _depthAttachment);
		switch (_depthAttachmentSpecification.TextureFormat)
		{
		case FrameBufferTextureFormat::DEPTH24STENCIL8:
			AttachDepthTexture(_depthAttachment, _specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, _specification.Width, _specification.Height);
			break;
		}
	}
	
	if (_colorAttachments.size() > 1)
	{
		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(_colorAttachments.size(), buffers);
	}
	else if (_colorAttachments.empty())
	{
		glDrawBuffer(GL_NONE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _rendererID);
	glViewport(0, 0, _specification.Width, _specification.Height);
}

void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0 || width > MaxFrameBufferSize || height > MaxFrameBufferSize)
		return;
	_specification.Width = width;
	_specification.Height = height;

	Invalidate();
}

int FrameBuffer::readPixel(uint32_t attachmentIndex, int x, int y)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	int32_t pixelData;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
	return pixelData;
}

void FrameBuffer::clearAttachment(uint32_t attachmentIndex, int value)
{
	auto& spec = _colorAttachmentSpecifications[attachmentIndex];
	glClearTexImage(_colorAttachments[attachmentIndex], 0, (GLenum)spec.TextureFormat, GL_INT, &value);
}

uint32_t FrameBuffer::getColorAttachmentRendererID(uint32_t index) const
{
	return _colorAttachments[index];
}

const FrameBufferSpecification& FrameBuffer::getSpecification() const
{
	return _specification;
}

// Temporarily

void FrameBuffer::draw()
{
	static float vert[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	unbind();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	_shader.bind();
	_shader.setUniform1i("u_Texture", 0);
	glBindTexture(GL_TEXTURE_2D, _colorAttachments[0]);
	VertexBuffer vb(vert, sizeof(vert), GL_STATIC_DRAW);
	vb.bind();
	VertexArray va;
	VertexBufferLayout vbl;
	vbl.push<float>(2);
	vbl.push<float>(2);
	va.addBuffer(vb, vbl);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
