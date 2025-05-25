#pragma once
#include "Renderer.h"
#include <array>
#include <string>

class CubeMap
{
private:
	unsigned int m_TextureID;
	int m_Width, m_Height, m_BPP;
	bool Moved = false;
public:
	CubeMap(std::array<std::string, 6> faces);
	CubeMap(CubeMap&& other) noexcept;
	CubeMap();

	~CubeMap();

	CubeMap& operator=(CubeMap&& other) noexcept;

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
};