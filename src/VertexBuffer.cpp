#include "VertexBuffer.h"
#include "GLError.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GL_CALL(glGenBuffers(1, &m_rendererID));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

}

VertexBuffer::~VertexBuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_rendererID));
}

void VertexBuffer::Bind() const
{
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void VertexBuffer::Unbind() const
{
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
