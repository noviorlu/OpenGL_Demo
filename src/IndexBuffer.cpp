#include "IndexBuffer.h"
#include "GLError.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) 
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
    GL_CALL(glGenBuffers(1, &m_rendererID));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));

}

IndexBuffer::~IndexBuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_rendererID));
}

void IndexBuffer::Bind() const
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
}

void IndexBuffer::Unbind() const
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
