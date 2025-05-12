#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer()
    :m_RendererID(0)
{
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    :m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
        if (Initialized && !Moved)
        {
            GLCall(glDeleteBuffers(1, &m_RendererID));
        }
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    this->m_RendererID = other.m_RendererID;
    this->m_Count = other.m_Count;
    this->Initialized = true;
    other.Moved = true;
    other.~IndexBuffer();

    return *this;
}

void IndexBuffer::Bind() const
{
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
