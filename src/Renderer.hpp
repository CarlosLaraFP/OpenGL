#pragma once

#include <GL/glew.h>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Material.hpp"

// Macro with compiler intrinsic to set breakpoints programatically
#define ASSERT(x) if (!(x)) __debugbreak();
// # in front of x turns it into a string
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

// OpenGL specifies all its enums in hexadecimal
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& vao, const IndexBuffer& ibo, Material& material) const;
};
