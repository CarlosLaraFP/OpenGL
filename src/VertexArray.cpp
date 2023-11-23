#include "VertexArray.hpp"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
    // Tells OpenGL that the VAO with the given ID is now in use, 
    // prompting the GPU to allocate memory and initialize the state for this VAO.
    this->Bind();
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

// Enables vertex for the currently bound VBO
void VertexArray::EnableVertexAttribute(unsigned int index, unsigned int componentCount, unsigned int componentBytes, GLenum componentType, GLboolean normalized, const GLvoid* offset)
{
    // Enables the vertex attribute array for attribute index 0, to be specified in glVertexAttribPointer.
    // This call is essential to activate the use of the specified vertex data during rendering.
    GLCall(glEnableVertexAttribArray(index));
    // Specifies how OpenGL should interpret the vertex data. Vertex attribute pointers tell OpenGL the layout of the vertex buffer.
    // Only one vertex attribute in this case (position), each with two floats (size == component count, not bytes), with vertex stride of 8 bytes.
    GLCall(glVertexAttribPointer(index, componentCount, componentType, normalized, componentCount * componentBytes, offset));
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
