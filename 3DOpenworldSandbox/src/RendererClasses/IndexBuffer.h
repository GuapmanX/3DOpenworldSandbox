#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
	bool Initialized = false;
	bool Moved = false;
public:
	IndexBuffer() = default;
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	IndexBuffer& operator=(IndexBuffer&& other) noexcept;
	IndexBuffer& operator=(IndexBuffer& other) noexcept;

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};