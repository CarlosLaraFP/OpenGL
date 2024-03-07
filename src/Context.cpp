#include "Context.hpp"
#include "Globals.hpp"

#include <iostream>
#include <stdexcept>

Context::Context()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        throw std::runtime_error("GLFW could not be initialized.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // Enforce the use of Vertex Array Objects (VAOs)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    m_Window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, "Graphics Programming", nullptr, nullptr);

    if (m_Window == nullptr)
    {
        glfwTerminate();

        throw std::runtime_error("Window could not be created.");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_Window);

    glfwSetFramebufferSizeCallback(m_Window, Context::OnWindowResize);

    // Vertical synchronization in the context of double buffering.
    // Synchronizes the buffer swap with the monitor's refresh rate and reduces unnecessary strain on the CPU and GPU.
    // Setting it to 1 on a 60Hz monitor caps the frame rate at 60 FPS.
    glfwSwapInterval(1);

    // We call glewInit() to initialize the extension entry points after creating a valid OpenGL rendering context.
    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("GLEW could not be initialized.");
    }

    // We now have access to all OpenGL functions of our driver's OpenGL version.
    std::cout << glGetString(GL_VERSION) << std::endl; // 4.6.0 NVIDIA 546.17 is the latest

    // Blending determines how to combine output color from the fragment shader with what is already in the target buffer
    glEnable(GL_BLEND);
    // source (new color) and destination (existing color)
    // the parameters are multiplication factors for each RGBA value for src and dest, respectively.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // GL_ONE and GL_ZERO by default: src pixels overwrite dest pixels (no translucency)
}

void Context::OnWindowResize(GLFWwindow* window, int newWidth, int newHeight)
{
    // Update the viewport (rectangular area of the window where OpenGL renders the image).
    // We use the entire window for rendering, so we set the viewport to cover the entire window. 
    // This is why x and y are set to 0, placing the lower left corner of the viewport at the lower left corner of the window.
    glViewport(0, 0, newWidth, newHeight);

    g_WindowWidth = newWidth;
    g_WindowHeight = newHeight;
    g_WindowResized = true;
}


Context::~Context()
{
    glfwTerminate();
}

GLFWwindow* Context::GetWindow() const
{
    return m_Window;
}

void Context::HandleCameraMovement()
{
    // Adjust speed as necessary
    if (glfwGetKey(m_Window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        g_CameraOffsetX += 0.01f;
    }
    if (glfwGetKey(m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        g_CameraOffsetX -= 0.01f;
    }
    if (glfwGetKey(m_Window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        g_CameraOffsetY -= 0.01f;
    }
    if (glfwGetKey(m_Window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        g_CameraOffsetY += 0.01f;
    }
}
