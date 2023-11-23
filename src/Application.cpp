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
    float vertexPositions[] =
    {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f  // 3
    };
    VertexBuffer vbo { vertexPositions, 4 * vertexComponents * sizeof(float) };
    // Here we add distinct vertex attributes (position, normals, texture coordinates, etc.) to specify the buffer layout
    vbo.Push<float>(vertexComponents);
    // Since there may be multiple vertex buffers, and multiple vertex attributes belonging to each, we need a more general AddBuffer
    vao.AddBuffer(vbo);

    unsigned int indices[]
    {
        0, 1, 2,
        2, 3, 0
    };
    IndexBuffer ibo { indices, 6 };

    // Up until here, the specific VAO encapsulates the state of the VBO and IBO. This remains even if the VAO is unbound.

    Shader shader { {"res/shaders/Basic.vert", "res/shaders/Basic.frag"} };

    // Unbind everything, starting with VAO
    vao.Unbind();
    // Shaders can unbound independently
    shader.Unbind();
    // When we unbind buffer objects after unbinding the VAO, it ensures that these unbinding actions don't alter the state of the VAO.
    // Therefore, re-binding buffer objects before every draw call is not necessary.
    vbo.Unbind();
    ibo.Unbind();

    float red = 0.0f; // Initialize color
    float increment = 0.02f;
    float rotationAngle = 0.0f; // Initialize rotation angle
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(context.GetWindow()))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        /*
            When you configure a VAO and subsequently use it in a draw call, OpenGL uses the state information stored in the VAO 
            to set up the necessary inputs for the vertex shader. This setup includes information about where in the GPU memory 
            the vertex data is, how it's formatted, and how it should be fed into the vertex shader's inputs.
        */
        shader.Bind();
        shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);
        shader.SetUniform1f("u_Rotation", rotationAngle);

        // Binding VAO is enough because it already encapsulates the VBO and IBO state.
        vao.Bind();

        // 6 indices, unsigned int enum, and pointer to indices not required because it's already bound (global state machine)
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // requires an index buffer
        /* macro expands to:
            GLClearError();
            glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);
            if (!(GLLogCall("glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr)", __FILE__, __LINE__))) __debugbreak();
        */

        red += increment;

        if (red > 1.0f) { increment = -0.02f; }
        else if (red < 0.0f) { increment = 0.02f; }

        // Increase by 5 degrees per frame
        rotationAngle += 2.0f;
        // Wrap around if it exceeds 360 degrees
        if (rotationAngle >= 360.0f) { rotationAngle -= 360.0f; }

        /* Swap front and back buffers */
        glfwSwapBuffers(context.GetWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Clean up resources
    // Destructors called in reverse order of creation at the end of this scope.

    return 0;
}
