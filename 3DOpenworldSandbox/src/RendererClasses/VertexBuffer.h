#pragma once
//#include "Renderer.h"


class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size, unsigned int usage = 35044); //GL_STATIC_DRAW
	~VertexBuffer();

	void SetBufferData(const void* buffer, unsigned int Offset, unsigned int BufferSize) const;
	void Bind() const;
	void Unbind() const;
};