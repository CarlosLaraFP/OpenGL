#include "VertexArray.hpp"
#include "Renderer.hpp"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

// Enables vertex for the currently bound VBO
void VertexArray::AddBuffer(const VertexBuffer& vbo)
{
    // Tells OpenGL that the VAO with the given ID is now in use, 
    // prompting the GPU to allocate memory and initialize the state for this VAO.
    this->Bind();
    // Lazy VBO binding
    vbo.Bind();
    const auto& elements = vbo.GetVertexAttributes();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); ++i)
    {
        // The index of the VertexBufferElement in the vector corresponds to the layout value in the vertex shader.
        const auto& element = elements[i];
        // Enables the vertex attribute array for attribute index i, to be specified in glVertexAttribPointer.
        // This call is essential to activate the use of the specified vertex data during rendering.
        GLCall(glEnableVertexAttribArray(i));
        // Specifies how OpenGL should interpret the vertex data. Vertex attribute pointers tell OpenGL the layout of the vertex buffer.
        // Only one vertex attribute in this case (position), each with two floats (size == component count, not bytes), with vertex stride of 8 bytes.
        GLCall(glVertexAttribPointer(
            i, 
            element.componentCount, 
            element.componentType, 
            element.normalized, 
            vbo.GetStride(),
            (const void*)offset
        ));

        // If a single VBO buffer contains more vertex attributes per vertex, the offset is the total memory jump from the last attribute.
        offset += element.componentCount * VertexBufferElement::GetSizeOfType(element.componentType);
    }

    // vbo.Unbind()?
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
