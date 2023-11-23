#include "VertexBuffer.hpp"

#include <stdexcept>

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    // Binding the buffer is essential for OpenGL to know which buffer we're working with in subsequent calls.
    // Any subsequent buffer operations that target GL_ARRAY_BUFFER will affect the buffer you've bound.
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    // Copies the vertex data (vertexPositions) to the GPU's memory for fast and efficient rendering.
    // When passing an array to a function, it decays into a pointer to its first element rather than being copied.
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

template<typename T>
void VertexBuffer::Push(unsigned int count)
{
    // Compile-time assertion unsupported in VS2022
    throw std::runtime_error("Unsupported type for VertexBuffer::Push");
}

template<>
void VertexBuffer::Push<float>(unsigned int count) 
{
    m_VertexAttributes.emplace_back(VertexAttribute{ GL_FLOAT, count, GL_FALSE });
    m_Stride += count * VertexAttribute::GetSizeOfType(GL_FLOAT);
}

template<>
void VertexBuffer::Push<unsigned int>(unsigned int count) 
{
    m_VertexAttributes.emplace_back(VertexAttribute{ GL_UNSIGNED_INT, count, GL_FALSE });
    m_Stride += count * VertexAttribute::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBuffer::Push<unsigned char>(unsigned int count) 
{
    m_VertexAttributes.emplace_back(VertexAttribute{ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_Stride += count * VertexAttribute::GetSizeOfType(GL_UNSIGNED_BYTE);
}
