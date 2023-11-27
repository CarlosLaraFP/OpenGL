#include "Square.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

Square::Square(glm::mat4 modelMatrix) : m_ModelMatrix { modelMatrix }
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
    vbo = new VertexBuffer { vertices, 4 * 2 * vertexComponents * sizeof(float) };
    // Here we add distinct vertex attributes (position, normals, texture coordinates, etc.) to specify the buffer layout
    vbo->Push<float>(vertexComponents); // vertex positions
    vbo->Push<float>(vertexComponents); // texture coordinates
    // Since there may be multiple vertex buffers, and multiple vertex attributes belonging to each, we need a more general AddBuffer
    vao->AddBuffer(*vbo);

    unsigned int indices[]
    {
        0, 1, 2,
        2, 3, 0
    };
    ibo = new IndexBuffer { indices, 6 };

    // Up until here, the specific VAO encapsulates the state of the VBO and IBO. This remains even if the VAO is unbound.

    material = new Material
    {
        Shader { {"res/shaders/Basic.vert", "res/shaders/Basic.frag"} },
        "res/textures/valinor.png",
        m_ModelMatrix,
        0.02f,
        2.0f
    };
}
