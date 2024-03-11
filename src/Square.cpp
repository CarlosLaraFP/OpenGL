#include "Square.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

Square::Square()
{
    SetLayout();
}

void Square::SetLayout()
{
    vao = new VertexArray{};
    /*
        When working with vertex shaders and rendering, we use Normalized Device Coordinates (NDC).
        In NDC, the coordinate system is defined where each axis has a range from -1 to 1,
        with the origin (0, 0, 0) at the center of the screen/window.
        Local space coordinates origin (0, 0) is at the center; texture coordinates origin (0, 0) is defined at the bottom left.
    */
    /*float vertices[] =
    {
        -0.5f, -0.5f, 0.0f, 0.0f, // 0
         0.5f, -0.5f, 1.0f, 0.0f, // 1
         0.5f,  0.5f, 1.0f, 1.0f, // 2
        -0.5f,  0.5f, 0.0f, 1.0f  // 3
    };*/
    //vbo = new VertexBuffer { vertices, sizeof(vertices), false }; // Static vertex buffer (readonly GPU memory)
    vbo = new VertexBuffer { nullptr, 1000, true }; // Dynamic vertex buffer can hold up to 1,000 vertices

    /*
    unsigned int indices[]
    {
        0, 1, 2, // triangle A
        2, 3, 0,  // triangle B
        4, 5, 6, // First triangle of the second square
        6, 7, 4  // Second triangle of the second square
    };*/
    ibo = new IndexBuffer{ nullptr, 1250, true }; // Dynamic index buffer can hold up to 1,250 indices

    // Up until here, the specific VAO encapsulates the state of the VBO and IBO. This remains even if the VAO is unbound.

    material = new Material
    {
        {"res/shaders/Basic.vert", "res/shaders/Basic.frag"},
        "res/textures/space.png", "res/textures/valinor.png",
        0.02f,
        2.0f
    };
}
