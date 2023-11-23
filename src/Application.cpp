#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
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
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

static std::string ParseShader(const std::string& filePath)
{
    std::ifstream stream { filePath };
    std::string line;
    std::stringstream source;

    while (getline(stream, line))
    {
        source << line << "\n";
    }

    return source.str();
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling

    int result;

    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;

        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // we want the message allocated on the stack
        char* message = (char*)alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

// The parameters are references to the actual source code (lifetimes > function scope)
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    // delete intermediate .obj files since program linking is done
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    Context context;

    // TODO: class Rectangle

    VertexArray vao;
    
    unsigned int vertexComponents = 2;
    /*
        When working with vertex shaders and rendering, we use Normalized Device Coordinates (NDC).
        In NDC, the coordinate system is defined where each axis has a range from -1 to 1,
        with the origin (0, 0, 0) at the center of the screen/window.
    */
    float vertexPositions[] =
    {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f  // 3
    };
    VertexBuffer vbo { vertexPositions, 4 * vertexComponents * sizeof(float) };
    vbo.Push<float>(vertexComponents);
    // Since there may be multiple vertex buffers, and multiple vertex attributes belonging to each, we need a more general AddBuffer
    vao.AddBuffer(vbo);

    unsigned int indices[]
    {
        0, 1, 2,
        2, 3, 0
    };
    IndexBuffer ibo { indices, 6 };

    // Up until here, the specific VAO encapsulates the state of the VBO and IBO. This remains even if the VAO is unbound.

    // Shaders
    auto vertexSource = ParseShader("res/shaders/Basic.vert");
    auto fragmentSource = ParseShader("res/shaders/Basic.frag");
    unsigned int shader = CreateShader(vertexSource, fragmentSource);
    GLCall(glUseProgram(shader));

    // Once the shader is created, every uniform gets assigned an ID so that we can reference it (lookup by name).
    GLCall(int colorLocation = glGetUniformLocation(shader, "u_Color"));
    ASSERT(colorLocation != -1);
    //GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));
    float red = 0.0f; // Initialize color
    float increment = 0.02f;

    GLCall(int rotationLocation = glGetUniformLocation(shader, "u_Rotation"));
    ASSERT(rotationLocation != -1);
    float rotationAngle = 0.0f; // Initialize rotation angle

    // Unbind everything, starting with VAO
    vao.Unbind();
    // Shaders can unbound independently
    GLCall(glUseProgram(0));
    // When we unbind buffer objects after unbinding the VAO, it ensures that these unbinding actions don't alter the state of the VAO.
    // Therefore, re-binding buffer objects before every draw call is not necessary.
    vbo.Unbind();
    ibo.Unbind();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(context.GetWindow()))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        /*
            When you configure a VAO and subsequently use it in a draw call, OpenGL uses the state information stored in the VAO 
            to set up the necessary inputs for the vertex shader. This setup includes information about where in the GPU memory 
            the vertex data is, how it's formatted, and how it should be fed into the vertex shader's inputs.
        */
        GLCall(glUseProgram(shader));
        // Uniforms are used to pass data from the CPU (C++ app) to the GPU (shader program).
        // Animate color (fragment shader). Uniforms are set per draw call.
        GLCall(glUniform4f(colorLocation, red, 0.3f, 0.8f, 1.0f));
        GLCall(glUniform1f(rotationLocation, rotationAngle));

        // Binding VAO is enough because it already encapsulates the VBO and IBO state.
        vao.Bind();

        // 6 indices, unsigned int enum, and pointer to indices not required because it's already bound (global state machine)
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // requires an index buffer
        /* macro expands to:
            GLClearError();
            glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);
            if (!(GLLogCall("glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr)", __FILE__, __LINE__))) __debugbreak();
        */

        red += increment;

        if (red > 1.0f) { increment = -0.02f; }
        else if (red < 0.0f) { increment = 0.02f; }

        // Increase by 5 degrees per frame
        rotationAngle += 2.0f;
        // Wrap around if it exceeds 360 degrees
        if (rotationAngle >= 360.0f) { rotationAngle -= 360.0f; }

        /* Swap front and back buffers */
        glfwSwapBuffers(context.GetWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Clean up resources
    GLCall(glDeleteProgram(shader));
    // Destructors called in reverse order of creation at the end of this scope.

    return 0;
}
