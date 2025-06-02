#pragma once


#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "VertexArray.h"


#include "vector"


class BufferObject
{
private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	bool Created = false;
public:
	BufferObject() = default;
	BufferObject(const void* VB, unsigned int VB_SIZE, unsigned int IB[], unsigned int IB_SIZE, std::vector<unsigned int> BufferLayout);
	~BufferObject();
	BufferObject& operator=(BufferObject&& other) noexcept;

	inline VertexArray& GetVA() { return m_VertexArray; };
	inline IndexBuffer& GetIB() { return m_IndexBuffer; };
};