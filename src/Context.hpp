#pragma once

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

class Context
{
private:
	GLFWwindow* m_Window;
    // Non-static member functions have an implicit this pointer as their first argument, which doesn't match the expected signature of GLFWframebuffersizefun.
    static void OnWindowResize(GLFWwindow* window, int width, int height);

public:
    Context();
	~Context();
    GLFWwindow* GetWindow() const;
    void HandleCameraMovement();
};
