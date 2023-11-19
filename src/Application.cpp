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

#include <iostream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "GLFW could not be initialized." << std::endl;
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // We call glewInit() to initialize the extension entry points after creating a valid OpenGL rendering context.
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW could not be initialized." << std::endl;
        return -1;
    }

    // We know have access to all OpenGL functions of our driver's OpenGL version.
    std::cout << glGetString(GL_VERSION) << std::endl; // 4.6.0 NVIDIA 546.17

    // Define vertex buffer
    float vertexPositions[6] =
    {
        -0.5f, -0.5,
         0.0f,  0.5f,
         0.5f, -0.5f
    };
    // Buffer objects are necessary to efficiently manage and store vertex data in GPU memory.
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    // Binding the buffer is essential for OpenGL to know which buffer we're working with in subsequent calls.
    // Any subsequent buffer operations that target GL_ARRAY_BUFFER will affect the buffer you've bound.
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // Uploads the vertex data (vertexPositions) to the GPU's memory for fast and efficient rendering.
    // When passing an array to a function, it decays into a pointer to its first element rather than being copied.
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertexPositions, GL_STATIC_DRAW);
    // Specifies how OpenGL should interpret the vertex data. Vertex attribute pointers tell OpenGL the layout of the vertex buffer.
    // Only one vertex attribute in this case (position), each with two floats (size == component count, not bytes), with vertex stride of 8 bytes.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    // Enables the vertex attribute array for attribute index 0, which was specified in glVertexAttribPointer.
    // This call is essential to activate the use of the specified vertex data during rendering.
    glEnableVertexAttribArray(0);
    // Up until here, the white triangle already renders because the NVIDIA GPU driver provides a default shader (program that runs on the GPU).

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Modern OpenGL requires a vertex [GPU memory (vRAM)] buffer and a [GPU] shader.
        glDrawArrays(GL_TRIANGLES, 0, 3); // does not require an index buffer
        //glDrawElements(GL_TRIANGLES, 3, ); // requires an index buffer

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
