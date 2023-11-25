#include "Texture.hpp"
#include "Renderer.hpp"

#include "stb_image/stb_image.h"

// TODO: Fully understand how this works

Texture::Texture(const std::string& path)
	: m_RendererID { 0 }, m_FilePath{ path }, m_LocalBuffer { nullptr }, 
	  m_Width { 0 }, m_Height { 0 }, m_BPP { 0 }, m_Slot { 0 }
{
	// Flipping the texture upside down because OpenGL expects PNG texture pixels to start at the bottom left (0, 0), not the top left.
	stbi_set_flip_vertically_on_load(1);

	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // 4 for RGBA

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// These must be specified
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Allocate our image data on the GPU
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	// Assumes we no longer need the texture data on the CPU
	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

Texture::Texture(Texture&& source) noexcept
{
	m_RendererID = source.m_RendererID;
	m_FilePath = source.m_FilePath;
	m_LocalBuffer = source.m_LocalBuffer;
	source.m_LocalBuffer = nullptr;
	m_Width = source.m_Width;
	m_Height = source.m_Height;
	m_BPP = source.m_BPP;
	m_Slot = source.m_Slot;
}

Texture& Texture::operator=(Texture&& source) noexcept
{
	if (this == &source) { return *this; }

	m_RendererID = source.m_RendererID;
	m_FilePath = source.m_FilePath;
	m_LocalBuffer = source.m_LocalBuffer;
	source.m_LocalBuffer = nullptr;
	m_Width = source.m_Width;
	m_Height = source.m_Height;
	m_BPP = source.m_BPP;
	m_Slot = source.m_Slot;

	return *this;
}

void Texture::Bind(unsigned int slot /*= 0*/)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	m_Slot = slot;
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
