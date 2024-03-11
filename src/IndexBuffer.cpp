#include "IndexBuffer.hpp"
#include "Renderer.hpp"

#include <vector>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int indexCount, bool dynamic) : m_IndexCount { indexCount }
{
    // Define index buffer to reuse vertices (must be unsigned)
    // Could use unsigned char instead of int (1 byte vs 4), but the range is only 0 - 255 (max # of indices).
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    // Update the subet of a buffer object's data. Index 0 because it's the entire buffer from the beginning.
    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_Indices.size() * sizeof(unsigned int), m_Indices.data()));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
