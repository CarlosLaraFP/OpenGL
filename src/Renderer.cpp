#include "Renderer.hpp"

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

// OpenGL specifies all its enums in hexadecimal
bool GLLogCall(const char* function, const char* file, int line)
{
    if (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, Material& material) const
{
    /*
        Binding VAO is enough because it already encapsulates the VBO state.
        When you configure a VAO and subsequently use it in a draw call, OpenGL uses the state information stored in the VAO
        to set up the necessary inputs for the vertex shader. This setup includes information about where in the GPU memory
        the vertex data is, how it's formatted, and how it should be fed into the vertex shader's inputs.
    */
    vao.Bind();
    ibo.Bind();
    material.Bind();

    // 6 indices, unsigned int enum, and pointer to indices not required because it's already bound (global state machine)
    GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr)); // requires an index buffer
    /* macro expands to:
        GLClearError();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        if (!(GLLogCall("glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr)", __FILE__, __LINE__))) __debugbreak();
    */
}
