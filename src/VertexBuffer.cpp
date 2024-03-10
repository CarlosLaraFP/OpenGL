#include "VertexBuffer.hpp"
#include "Renderer.hpp"

#include <array>

VertexBuffer::VertexBuffer(const void* data, unsigned int size, bool dynamic)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    // Binding the buffer is essential for OpenGL to know which buffer we're working with in subsequent calls.
    // Any subsequent buffer operations that target GL_ARRAY_BUFFER will affect the buffer you've bound.
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    // Copies the vertex data (vertexPositions) to the GPU's memory for fast and efficient rendering.
    // When passing an array to a function, it decays into a pointer to its first element rather than being copied.
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind()
{
    // (px, py), (tx, ty), (r, g, b, a), (t)
    /*float vertices[] =
    {
        // First square (left)
        -0.5f - 0.5f, -0.5f, 0.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, // 0
         0.5f - 0.5f, -0.5f, 1.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, // 1
         0.5f - 0.5f,  0.5f, 1.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, // 2
        -0.5f - 0.5f,  0.5f, 0.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, // 3

        // Second square (right)
        -0.5f + 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, // 4
         0.5f + 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, // 5
         0.5f + 0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, // 6
        -0.5f + 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f  // 7
    };*/

    auto q0 = CreateQuad(-0.5f, 0.0f, 0.0f);
    auto q1 = CreateQuad( 0.5f, 0.0f, 1.0f);

    Vertex vertices[8];
    memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
    memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    // Update the subet of a buffer object's data. Index 0 because it's the entire buffer from the beginning.
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

    // Specifies how OpenGL should interpret the vertex data. 
    // Vertex attribute pointers tell OpenGL the layout of the vertex buffer.

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex), // total vertex size
        (const void*)offsetof(Vertex, Position) // bytes memory offset to reach attribute within vertex
    ));

    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex), // total vertex size
        (const void*)offsetof(Vertex, TextureCoordinates) // bytes memory offset to reach attribute within vertex
    ));

    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(
        2,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex), // total vertex size
        (const void*)offsetof(Vertex, Color) // bytes memory offset to reach attribute within vertex
    ));

    GLCall(glEnableVertexAttribArray(3));
    GLCall(glVertexAttribPointer(
        3,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex), // total vertex size
        (const void*)offsetof(Vertex, TextureID) // bytes memory offset to reach attribute within vertex
    ));

    // vbo.Unbind()?
}

// Enables vertex for the currently bound VBO
void VertexBuffer::AddVertices()
{
    unsigned int offset = 0;

    for (unsigned int i = 0; i < m_VertexAttributes.size(); ++i)
    {
        // The index of the VertexAttribute in the vector corresponds to the layout value in the vertex shader.
        const auto& vertexAttribute = m_VertexAttributes[i];
        // Enables the vertex attribute array for attribute index i, to be specified in glVertexAttribPointer.
        // This call is essential to activate the use of the specified vertex data during rendering.
        GLCall(glEnableVertexAttribArray(i));
        // Specifies how OpenGL should interpret the vertex data. Vertex attribute pointers tell OpenGL the layout of the vertex buffer.
        // Only one vertex attribute in this case (position), each with two floats (size == component count, not bytes), with vertex stride of 8 bytes.
        GLCall(glVertexAttribPointer(
            i,
            vertexAttribute.componentCount,
            vertexAttribute.componentType,
            vertexAttribute.normalized,
            this->GetStride(), // total vertex size
            (const void*)offset // bytes memory offset to reach attribute within vertex (TODO based on struct members)
        ));

        // If a single VBO buffer contains more vertex attributes per vertex, the offset is the total memory jump from the last attribute.
        offset += vertexAttribute.componentCount * VertexAttribute::GetSizeOfType(vertexAttribute.componentType);
    }

    // vbo.Unbind()?
}

// (px, py), (tx, ty), (r, g, b, a), (t)
std::array<Vertex, 4> VertexBuffer::CreateQuad(float x, float y, float t)
{
    Vertex v0;
    v0.Position = { -0.5f + x, -0.5f + y };
    v0.TextureCoordinates = { 0.0f, 0.0f };
    v0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v0.TextureID = t;

    Vertex v1;
    v1.Position = { 0.5f + x, -0.5f + y };
    v1.TextureCoordinates = { 1.0f, 0.0f };
    v1.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v1.TextureID = t;

    Vertex v2;
    v2.Position = { 0.5f + x, 0.5f + y };
    v2.TextureCoordinates = { 1.0f, 1.0f };
    v2.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v2.TextureID = t;

    Vertex v3;
    v3.Position = { -0.5f + x, 0.5f + y };
    v3.TextureCoordinates = { 0.0f, 1.0f };
    v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v3.TextureID = t;

    return { v0, v1, v2, v3 };
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
