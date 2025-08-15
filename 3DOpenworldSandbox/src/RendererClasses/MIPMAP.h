#pragma once

#include "Renderer.h"
#include <vector>
#include <string>

class MIPMAP
{
private:
	unsigned int m_TextureID;
	int m_Width, m_Height, m_BPP;
	bool Initialized = false;
	bool Moved = false;
public:
	MIPMAP(std::vector<std::string> Levels);
	MIPMAP(MIPMAP&& other) noexcept;
	MIPMAP() = default;

	~MIPMAP();

	MIPMAP& operator=(MIPMAP&& other) noexcept;

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
};