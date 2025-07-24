#include "VertexBuffer.h"

#include "Renderer.h"
#include <iostream>

VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int usage)//GLenum
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
    Initialized = true;
}

/*VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
{
    this->m_RendererID = other.m_RendererID;
    other.m_RendererID = 0;
    other.Moved = true;
    other.~VertexBuffer();
    this->Initialized = true;
}*/

VertexBuffer::~VertexBuffer()
{
    if (Initialized && !Moved)
    {
        std::cout << "deleted" << std::endl;
        GLCall(glDeleteBuffers(1, &m_RendererID));
    }
}

//This should fix overriding the old class by just replacing the id(which is the only unique thing in buffers)
VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    this->m_RendererID = other.m_RendererID;
    other.m_RendererID = 0;
    other.Moved = true;
    other.~VertexBuffer();
    this->Initialized = true;

    return *this;
}
VertexBuffer& VertexBuffer::operator=(VertexBuffer& other) noexcept
{
    // TODO: insert return statement here
    this->m_RendererID = other.m_RendererID;
    other.m_RendererID = 0;
    other.Moved = true;
    other.~VertexBuffer();
    this->Initialized = true;

    return *this;
}
///////

void VertexBuffer::SetBufferData(const void* buffer, unsigned int Offset, unsigned int BufferSize) const
{
    Bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, Offset, BufferSize, buffer));
    Unbind();
}

void VertexBuffer::Bind() const
{
    //if (Initialized)
    //{
     GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    //}
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

