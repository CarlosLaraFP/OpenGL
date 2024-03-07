#include "Square.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

Square::Square()
{
    vao = new VertexArray{};

    SetLayout();
}

void Square::SetLayout()
{
    unsigned int vertexComponents = 2;
    /*
        When working with vertex shaders and rendering, we use Normalized Device Coordinates (NDC).
        In NDC, the coordinate system is defined where each axis has a range from -1 to 1,
        with the origin (0, 0, 0) at the center of the screen/window.
        Local space coordinates origin (0, 0) is at the center; texture coordinates origin (0, 0) is defined at the bottom left.
    */
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f, 0.0f, // 0
         0.5f, -0.5f, 1.0f, 0.0f, // 1
         0.5f,  0.5f, 1.0f, 1.0f, // 2
        -0.5f,  0.5f, 0.0f, 1.0f  // 3
    };

    std::vector<float> doubledVertices;
    // Call function to create double squares with 1.0 unit spacing
    CreateDoubleSquares(vertices, sizeof(vertices) / sizeof(vertices[0]), 1.0f, doubledVertices);

    vbo = new VertexBuffer { doubledVertices.data(), 8 * 2 * vertexComponents * sizeof(float) };
    // Here we add distinct vertex attributes (position, normals, texture coordinates, etc.) to specify the buffer layout
    vbo->Push<float>(vertexComponents); // vertex positions
    vbo->Push<float>(vertexComponents); // texture coordinates
    // Since there may be multiple vertex buffers, and multiple vertex attributes belonging to each, we need a more general AddBuffer
    vao->AddBuffer(*vbo);

    unsigned int indices[]
    {
        0, 1, 2, // triangle A
        2, 3, 0,  // triangle B
        4, 5, 6, // First triangle of the second square
        6, 7, 4  // Second triangle of the second square
    };
    ibo = new IndexBuffer { indices, 3 * 2 * 2 };

    // Up until here, the specific VAO encapsulates the state of the VBO and IBO. This remains even if the VAO is unbound.

    material = new Material
    {
        {"res/shaders/Basic.vert", "res/shaders/Basic.frag"},
        "res/textures/space.png",
        0.02f,
        2.0f
    };
}

void Square::CreateDoubleSquares(const float originalVertices[], size_t originalSize, float spacing, std::vector<float>& newVertices) {
    // Clear the vector that will hold the new vertices
    newVertices.clear();

    // Determine the number of vertices in the original array
    size_t numVertices = originalSize / 4; // Since each vertex has 4 components (x, y, u, v)

    // Copy original vertices to newVertices vector, adjusting for the left square
    for (size_t i = 0; i < numVertices; ++i) {
        newVertices.push_back(originalVertices[i * 4] - spacing / 2); // Adjust X
        newVertices.push_back(originalVertices[i * 4 + 1]);           // Y stays the same
        newVertices.push_back(originalVertices[i * 4 + 2]);           // U (texture coord)
        newVertices.push_back(originalVertices[i * 4 + 3]);           // V (texture coord)
    }

    // Copy again for the right square, adjusting positions
    for (size_t i = 0; i < numVertices; ++i) {
        newVertices.push_back(originalVertices[i * 4] + spacing / 2); // Adjust X
        newVertices.push_back(originalVertices[i * 4 + 1]);           // Y stays the same
        newVertices.push_back(originalVertices[i * 4 + 2]);           // U (texture coord)
        newVertices.push_back(originalVertices[i * 4 + 3]);           // V (texture coord)
    }
}
