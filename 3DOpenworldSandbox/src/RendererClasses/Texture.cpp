#include "Texture.h"
#include "stb_image/stb_image.h"
#include <iostream>

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
	m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); //better for bigger textures, but bad for pixel art

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST)); //More suitable format for mipmaps
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); //GL_CLAMP_TO_EDGE
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	//GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE)); //not needed

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	//GLCall(glGenerateMipmap(GL_TEXTURE_2D)); //generates a mipmap
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}

	Initialized = true;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	this->m_RendererID = other.m_RendererID;
	this->m_Height = other.m_Height;
	this->m_Width = other.m_Width;
	this->m_BPP = other.m_BPP;
	other.m_RendererID = 0;
	other.Moved = true;
	this->Initialized = true;

	return *this;
}

Texture::Texture(Texture&& other) noexcept
{
	this->m_RendererID = other.m_RendererID;
	this->m_Height = other.m_Height;
	this->m_Width = other.m_Width;
	this->m_BPP = other.m_BPP;
	other.m_RendererID = 0;
	other.Moved = true;
	this->Initialized = true;
}

Texture::~Texture()
{
	if (Initialized && !Moved)
		GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	if (Initialized && !Moved) {
		std::cout << m_RendererID << std::endl;
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}
}

void Texture::Unbind() const
{
	if (Initialized && !Moved) {
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
}
