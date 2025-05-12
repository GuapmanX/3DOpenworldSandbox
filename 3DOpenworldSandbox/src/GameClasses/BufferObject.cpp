#include "BufferObject.h"

BufferObject::BufferObject(const void* VB, unsigned int VB_SIZE, unsigned int IB[], unsigned int IB_SIZE, std::vector<unsigned int> BufferLayout)
{
	//VertexBuffer VertB(VB, VB_SIZE);
    m_VertexBuffer = VertexBuffer(VB, VB_SIZE);


    VertexBufferLayout layout;
    //layout.Push<float>(3);//position
    //layout.Push<float>(3);//color
    //layout.Push<float>(2);//texcoords

    for (int i = 0; i < (int)BufferLayout.size(); i++)
    {
        layout.Push<float>(BufferLayout[i]);
    }


    m_VertexArray.AddBuffer(m_VertexBuffer, layout);

    m_IndexBuffer = IndexBuffer(IB, IB_SIZE);





    m_VertexArray.Unbind();
    m_IndexBuffer.Unbind();
    m_IndexBuffer.Unbind();
};

BufferObject::~BufferObject()
{
};
