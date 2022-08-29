#include "VertexArray.h"
#include "BufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(const Buffer& b, const BufferLayout& layout)
{
	
	b.Bind();

	const auto& elements = layout.getElements();

	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i) 
	{
		
		const auto& element = elements[i];

		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, element.count * element.GetSizeOfType(element.type), (const void*)offset));
		offset += element.count * element.GetSizeOfType(element.type);

	}

}
