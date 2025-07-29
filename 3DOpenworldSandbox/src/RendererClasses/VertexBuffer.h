#pragma once
//#include "Renderer.h"


class VertexBuffer
{
private:
	unsigned int m_RendererID;
	bool Initialized = false;
	bool Moved = false;
public:
	VertexBuffer(const void* data, unsigned int size, unsigned int usage = 35044); //GL_STATIC_DRAW
	VertexBuffer() = default;
	VertexBuffer(VertexBuffer&& other) noexcept;
	~VertexBuffer();

	VertexBuffer& operator=(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer& other) noexcept;

	void SetBufferData(const void* buffer, unsigned int Offset, unsigned int BufferSize) const;
	void Bind() const;
	void Unbind() const;
};