#include "Renderer.hpp"
#include "Geometry.hpp"

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

void Renderer::Draw(Geometry& geometry) const
{
    geometry.Bind();

    // 6 indices, unsigned int enum, and pointer to indices not required because it's already bound (global state machine)
    GLCall(glDrawElements(GL_TRIANGLES, geometry.GetIndexCount(), GL_UNSIGNED_INT, nullptr)); // requires an index buffer
    /* macro expands to:
        GLClearError();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        if (!(GLLogCall("glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr)", __FILE__, __LINE__))) __debugbreak();
    */
}
