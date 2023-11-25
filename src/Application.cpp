#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
/*
    GLEW is used to interface with this machine's GPU drivers to access NVIDIA's OpenGL code implementation.
    It simplifies the process of accessing advanced features and extensions in OpenGL that are not included in the standard OpenGL distribution.
    Must be included before GLFW.
*/
#include <GL/glew.h>
/*
    GLFW is essentially a cross-platform library that provides an abstraction over the operating-system-specific APIs for creating windows, 
    handling input, event polling, time measurement, and managing OpenGL contexts. This abstraction allows OpenGL developers to write code 
    that is largely platform-independent, avoiding the need to directly interact with the low-level, platform-specific APIs.
*/
#include <GLFW/glfw3.h>
/*
    Use docs.gl for OpenGL documentation and in-depth understanding. Its mastery is key.
    Avoid creating functions and variables with OpenGL types; use C++
*/
#include "Context.hpp"
#include "Renderer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Texture.hpp"

int main(void)
{
    Context context;

    // TODO: class Rectangle

    VertexArray vao;
    
    unsigned int vertexComponents = 2;
    /*
        When working with vertex shaders and rendering, we use Normalized Device Coordinates (NDC).
        In NDC, the coordinate system is defined where each axis has a range from -1 to 1,
        with the origin (0, 0, 0) at the center of the screen/window.
    */
    // local space coordinates origin (0, 0) is at the center; texture coordinates origin (0, 0) is defined at the bottom left
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f, 0.0f, // 0
         0.5f, -0.5f, 1.0f, 0.0f, // 1
         0.5f,  0.5f, 1.0f, 1.0f, // 2
        -0.5f,  0.5f, 0.0f, 1.0f  // 3
    };
    VertexBuffer vbo { vertices, 4 * 2 * vertexComponents * sizeof(float) };
    // Here we add distinct vertex attributes (position, normals, texture coordinates, etc.) to specify the buffer layout
    vbo.Push<float>(vertexComponents); // vertex positions
    vbo.Push<float>(vertexComponents); // texture coordinates
    // Since there may be multiple vertex buffers, and multiple vertex attributes belonging to each, we need a more general AddBuffer
    vao.AddBuffer(vbo);

    unsigned int indices[]
    {
        0, 1, 2,
        2, 3, 0
    };
    IndexBuffer ibo { indices, 6 };

    // Up until here, the specific VAO encapsulates the state of the VBO and IBO. This remains even if the VAO is unbound.

    // Unbind everything, starting with VAO
    //vao.Unbind();
    // When we unbind buffer objects after unbinding the VAO, it ensures that these unbinding actions don't alter the state of the VAO.
    // Therefore, re-binding buffer objects before every draw call is not necessary.
    //vbo.Unbind();
    //ibo.Unbind();
    // Shaders can unbound independently
    //shader.Unbind();

    /*
    Material material 
    { 
        Shader { {"res/shaders/Basic.vert", "res/shaders/Basic.frag"} },
        0.02f, 
        2.0f
    };
    */
    
    
    Shader shader { {"res/shaders/Basic.vert", "res/shaders/Basic.frag"} };
    shader.Bind();
    Texture texture { "res/textures/valinor.png" };
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);
    
    Renderer renderer;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(context.GetWindow()))
    {
        /* Render here */
        renderer.Clear();

        shader.Bind();

        renderer.Draw(vao, ibo);

        /* Swap front and back buffers */
        glfwSwapBuffers(context.GetWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Clean up resources
    // Destructors called in reverse order of creation at the end of this scope.

    return 0;
}
