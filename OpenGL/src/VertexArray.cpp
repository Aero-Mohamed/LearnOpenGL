#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GlCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GlCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::bind() const
{
	GlCall(glBindVertexArray(m_RendererID));
}

void VertexArray::unbind() const
{
	GlCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	this->bind();
	vb.bind();
	
	unsigned int offset = 0;
	const auto& elements = layout.getElements();

	for (int i = 0; i < elements.size(); i++) {
		
		const auto& element = elements[i];
		
		GlCall(glEnableVertexAttribArray(i));
		GlCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, 
			layout.getStride(), (const void*)offset));
		
		offset += (element.count * VertexBufferElement::getTypeSize(element.type));
	}
}
