#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;
	bool moved = false;
	bool Initialized = false;
public:
	VertexArray() = default;
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb,const VertexBufferLayout& layout);

	VertexArray& operator=(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray& other) noexcept;
	VertexArray(VertexArray&& other);

	void Bind() const;
	void Unbind() const;
};