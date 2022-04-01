#include "Renderer.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GlCall(glGenBuffers(1, &m_RendererID));
    this->bind();
    GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GlCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::bind() const
{
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unbind() const
{
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
