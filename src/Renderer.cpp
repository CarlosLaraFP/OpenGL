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

/*
    A single frame might involve multiple draw calls, depending on the complexity of the scene and how it's organized. Each draw call is an instruction to the GPU to render a set of primitives (e.g., triangles, lines) based on the currently bound state, which includes shaders, textures, and other resources.

    Here’s what typically happens in a single frame in an OpenGL application aiming for 60 FPS:

    Clearing Buffers: At the beginning of a frame, color, depth, and possibly stencil buffers are cleared to prepare for new drawing commands.

    Setting State: Program state is set up, including binding shaders, setting uniforms, and configuring rendering state (like blending, depth testing).

    Issuing Draw Calls: Multiple draw calls are made to render various parts of the scene. This could include drawing the terrain, characters, UI elements, etc. The number of draw calls per frame can vary greatly from a few dozen to thousands, depending on the application and how it manages rendering.

    Swapping Buffers: Once all drawing is complete, the buffers are swapped (in double-buffering mode), presenting the rendered frame to the screen.

    To achieve 60 FPS, the entire process from clearing buffers to swapping buffers needs to complete within approximately 16.67 milliseconds (1 second / 60). This includes not just the draw calls but also any computation, state changes, buffer updates, and other work done per frame.

    Optimization
    Batching: Minimizing the number of draw calls through techniques like batching, where multiple objects are rendered in a single draw call by combining their geometry or using instanced rendering.

    State Changes: Reducing the overhead of state changes between draw calls by organizing rendering tasks to minimize shader, texture, and render target switches.

    Culling: Implementing frustum and occlusion culling to avoid rendering objects that are not visible in the current view.

    Level of Detail (LOD): Adjusting the detail of rendered objects based on their distance from the camera to reduce the number of vertices and textures needed.

    Achieving high frame rates is about the efficiency of the entire rendering pipeline and how effectively you can utilize the GPU to perform drawing operations within the time budget of each frame.
*/
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
