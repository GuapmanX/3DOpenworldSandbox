#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include <iostream>
//VertexArray::VertexArray()
//{
	//GLCall(glGenVertexArrays(1, &m_RendererID));
//}

/*VertexArray::VertexArray(bool Generate)
{
	if (Generate)
		GLCall(glGenVertexArrays(1, &m_RendererID));
}*/

VertexArray::~VertexArray()
{
	if (!moved && Initialized)
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{

	GLCall(glGenVertexArrays(1, &m_RendererID));
	Initialized = true;

	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));

		if (element.type == GL_INT || element.type == GL_UNSIGNED_INT)
		{
			GLCall(glVertexAttribIPointer(i, element.count, element.type, layout.GetStride(), (const void*)offset));
		}
		else
		{
			GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		}

		//GLCall(glVertexAttribPointer(i, element.count , element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	// TODO: insert return statement here
	this->m_RendererID = other.m_RendererID;
	this->Initialized = true;
	other.moved = true;
	other.~VertexArray();

	return *this;
}

VertexArray& VertexArray::operator=(VertexArray& other) noexcept
{
	// TODO: insert return statement here
	this->m_RendererID = other.m_RendererID;
	this->Initialized = true;
	other.moved = true;
	other.~VertexArray();

	return *this;
}

VertexArray::VertexArray(VertexArray&& other)
{
	this->m_RendererID = other.m_RendererID;
	other.moved = true;
}

void VertexArray::Bind() const
{
	if (Initialized)
	{
		GLCall(glBindVertexArray(m_RendererID));
	}
}

void VertexArray::Unbind() const
{
	if (Initialized)
	{
		GLCall(glBindVertexArray(0));
	}
}
