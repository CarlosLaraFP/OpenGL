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
#include "Square.hpp"
#include "Globals.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// TODO: Replace with a safer approach
int g_WindowWidth = 640;
int g_WindowHeight = 480;
bool g_WindowResized = true;

int main(void)
{
    Context context;

    Square square;
    
    Renderer renderer;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(context.GetWindow()))
    {
        /* Render here */
        renderer.Clear();
        renderer.Draw(square);

        /* Swap front and back buffers */
        glfwSwapBuffers(context.GetWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Clean up resources
    // Destructors called in reverse order of creation at the end of this scope.

    return 0;
}
