#include "MIPMAP.h"
#include "stb_image/stb_image.h"
#include <iostream>

MIPMAP::MIPMAP(std::vector<std::string> Levels)
	:m_Width(0), m_Height(0), m_BPP(0), m_TextureID(0)
{

	GLCall(glGenTextures(1, &m_TextureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));

	unsigned char* m_LocalBuffer = nullptr;
	stbi_set_flip_vertically_on_load(1);


	for (unsigned int level = 0; level < Levels.size(); level++)
	{
		unsigned char* m_LocalBuffer = stbi_load(Levels[level].c_str(), &m_Width, &m_Height, &m_BPP, 0);

		if (m_LocalBuffer)
		{
			GLCall(glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
			stbi_image_free(m_LocalBuffer);
		}
		else
		{
			std::cout << "MIPMAP failed to load at path: " << Levels[level] << std::endl;
			stbi_image_free(m_LocalBuffer);
		}


	}


	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST)); //More suitable format for mipmaps
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); //GL_CLAMP_TO_EDGE
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);       // first mip level
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, Levels.size() - 1); // last mip level

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}

	Initialized = true;
}




MIPMAP::~MIPMAP()
{
	if (Initialized && !Moved)
		GLCall(glDeleteTextures(1, &m_TextureID));
}

MIPMAP& MIPMAP::operator=(MIPMAP&& other) noexcept
{
	this->m_TextureID = other.m_TextureID;
	this->m_Height = other.m_Height;
	this->m_Width = other.m_Width;
	this->m_BPP = other.m_BPP;
	other.m_TextureID = 0;
	other.Moved = true;
	this->Initialized = true;

	return *this;
}

MIPMAP::MIPMAP(MIPMAP&& other) noexcept
{
	this->m_TextureID = other.m_TextureID;
	this->m_Height = other.m_Height;
	this->m_Width = other.m_Width;
	this->m_BPP = other.m_BPP;
	other.m_TextureID = 0;
	other.Moved = true;
	this->Initialized = true;
}

void MIPMAP::Bind(unsigned int slot) const
{
	if (Initialized && !Moved) {
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
	}
}

void MIPMAP::Unbind() const
{
	if (Initialized && !Moved) 
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}