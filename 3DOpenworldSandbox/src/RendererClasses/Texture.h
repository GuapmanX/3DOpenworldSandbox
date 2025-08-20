#pragma once

#include "Renderer.h"
#include <string>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
	bool Initialized = false;
	bool Moved = false;
public:
	Texture() = default;
	Texture(const std::string& path, int WrapMethod = GL_NEAREST);
	~Texture();

	Texture& operator=(Texture&& other) noexcept;
	Texture(Texture&& other) noexcept;

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};