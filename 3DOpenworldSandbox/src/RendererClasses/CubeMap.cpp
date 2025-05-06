#include "CubeMap.h"
#include "stb_image/stb_image.h"
#include <iostream>

CubeMap::CubeMap(std::array<std::string, 6> faces)
	:m_Width(0), m_Height(0), m_BPP(0)
{
	GLCall(glGenTextures(1, &m_TextureID));
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

	//stbi_set_flip_vertically_on_load(1);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &m_Width, &m_Height, &m_BPP, 0);

		if (data)
		{
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
		}


	}
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));


	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

CubeMap::~CubeMap()
{
	GLCall(glDeleteTextures(1, &m_TextureID));
}

void CubeMap::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
}

void CubeMap::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}
