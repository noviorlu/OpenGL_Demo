#include "glpch.h"
#include "IndexBuffer.h"

namespace GLCore::Utils {

    IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
        : m_Count(count)
    {
        GLCORE_ASSERT(sizeof(unsigned int) == sizeof(GLuint), "Opengl Uint size not equals to computer Uint size!");
        glGenBuffers(1, &m_rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);

    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_rendererID);
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
    }

    void IndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}