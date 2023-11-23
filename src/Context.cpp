#include "Context.hpp"

#include <iostream>
#include <stdexcept>

Context::Context()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        throw std::runtime_error("GLFW could not be initialized.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enforce the use of Vertex Array Objects (VAOs)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    m_Window = glfwCreateWindow(640, 480, "Graphics Programming", nullptr, nullptr);

    if (m_Window == nullptr)
    {
        glfwTerminate();

        throw std::runtime_error("Window could not be created.");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_Window);

    // Vertical synchronization in the context of double buffering.
    // Synchronizes the buffer swap with the monitor's refresh rate and reduces unnecessary strain on the CPU and GPU.
    // Setting it to 1 on a 60Hz monitor caps the frame rate at 60 FPS.
    glfwSwapInterval(1);

    // We call glewInit() to initialize the extension entry points after creating a valid OpenGL rendering context.
    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("GLEW could not be initialized.");
    }

    // We know have access to all OpenGL functions of our driver's OpenGL version.
    std::cout << glGetString(GL_VERSION) << std::endl; // 4.6.0 NVIDIA 546.17 is the latest
}

Context::~Context()
{
    glfwTerminate();
}

GLFWwindow* Context::GetWindow() const
{
    return m_Window;
}
