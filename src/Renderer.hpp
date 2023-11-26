#pragma once

#include <GL/glew.h>

#include "Geometry.hpp"

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
    // TODO: Take in abstract class Geometry with appropriate virtual methods
    void Draw(Geometry& geometry) const;
};
