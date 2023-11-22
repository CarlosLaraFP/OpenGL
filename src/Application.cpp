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

// Macro with compiler intrinsic to set breakpoints programatically
#define ASSERT(x) if (!(x)) __debugbreak();
// # in front of x turns it into a string
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

// OpenGL specifies all its enums in hexadecimal
static bool GLLogCall(const char* function, const char* file, int line)
{
    if (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream stream { filePath };
    std::string line;
    std::stringstream source[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            source[(int)type] << line << "\n";
        }
    }

    return { source[0].str(), source[1].str() };
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
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "GLFW could not be initialized." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enforce the use of Vertex Array Objects (VAOs)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Vertical synchronization in the context of double buffering.
    // Synchronizes the buffer swap with the monitor's refresh rate and reduces unnecessary strain on the CPU and GPU.
    // Setting it to 1 on a 60Hz monitor caps the frame rate at 60 FPS.
    glfwSwapInterval(1);

    // We call glewInit() to initialize the extension entry points after creating a valid OpenGL rendering context.
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW could not be initialized." << std::endl;
        return -1;
    }

    // We know have access to all OpenGL functions of our driver's OpenGL version.
    std::cout << glGetString(GL_VERSION) << std::endl; // 4.6.0 NVIDIA 546.17 is the latest

    /*
        When working with vertex shaders and rendering, we use Normalized Device Coordinates (NDC). 
        In NDC, the coordinate system is defined where each axis has a range from -1 to 1, 
        with the origin (0, 0, 0) at the center of the screen/window.
    */

    // Define vertex buffer
    float vertexPositions[] =
    {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f  // 3
    };

    /*
        The VAO is a GPU-side object that holds the state needed to supply vertex data to vertex shaders. 
        The link between the attributes defined in the VAO and those in the shader is made through the attribute indices. 
    */
    unsigned int vertexArrayObject;
    GLCall(glGenVertexArrays(1, &vertexArrayObject));
    /*
        Tells OpenGL that the VAO with the given ID is now in use, prompting the GPU to allocate memory and initialize the state for this VAO.
    */
    GLCall(glBindVertexArray(vertexArrayObject));

    // Buffer objects are necessary to efficiently manage and store vertex data in GPU memory.
    unsigned int vertexBufferObject;
    GLCall(glGenBuffers(1, &vertexBufferObject));
    // Binding the buffer is essential for OpenGL to know which buffer we're working with in subsequent calls.
    // Any subsequent buffer operations that target GL_ARRAY_BUFFER will affect the buffer you've bound.
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject));
    // Copies the vertex data (vertexPositions) to the GPU's memory for fast and efficient rendering.
    // When passing an array to a function, it decays into a pointer to its first element rather than being copied.
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertexPositions, GL_STATIC_DRAW));
    // Enables the vertex attribute array for attribute index 0, to be specified in glVertexAttribPointer.
    // This call is essential to activate the use of the specified vertex data during rendering.
    GLCall(glEnableVertexAttribArray(0));
    // Specifies how OpenGL should interpret the vertex data. Vertex attribute pointers tell OpenGL the layout of the vertex buffer.
    // Only one vertex attribute in this case (position), each with two floats (size == component count, not bytes), with vertex stride of 8 bytes.
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
    // Up until here, the specific VAO encapsulates the state of the VBO. This remains even if the VAO is unbound.

    // Define index buffer to reuse vertices (must be unsigned)
    // Could use unsigned char instead of int (1 byte vs 4), but the range is only 0 - 255 (max # of indices).
    unsigned int indices[]
    {
        0, 1, 2,
        2, 3, 0
    };
    // Set index buffer state
    unsigned int indexBufferObject;
    GLCall(glGenBuffers(1, &indexBufferObject));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    // Shaders
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    // Once the shader is created, every uniform gets assigned an ID so that we can reference it (lookup by name).
    GLCall(int colorLocation = glGetUniformLocation(shader, "u_Color"));
    ASSERT(colorLocation != -1);
    //GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));
    float red = 0.0f; // Initialize color
    float increment = 0.05f;

    GLCall(int rotationLocation = glGetUniformLocation(shader, "u_Rotation"));
    ASSERT(rotationLocation != -1);
    float rotationAngle = 0.0f; // Initialize rotation angle

    // Unbind everything, starting with VAO
    GLCall(glBindVertexArray(0));
    // Shaders can unbound independently
    GLCall(glUseProgram(0));
    // When we unbind buffer objects after unbinding the VAO, it ensures that these unbinding actions don't alter the state of the VAO.
    // Therefore, re-binding buffer objects before every draw call is not necessary.
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
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
        GLCall(glBindVertexArray(vertexArrayObject));

        // Modern OpenGL requires a vertex [GPU memory (vRAM)] buffer and a [GPU] shader.
        //glDrawArrays(GL_TRIANGLES, 0, 3); // does not require an index buffer
        // 6 indices, unsigned int enum, and pointer to indices not required because it's already bound (global state machine)
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // requires an index buffer
        /*
        GLClearError();
        glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);
        if (!(GLLogCall("glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr)", __FILE__, __LINE__))) __debugbreak();
        */

        red += increment;

        if (red > 1.0f) { increment = -0.05f; }
        else if (red < 0.0f) { increment = 0.05; }

        // Increase by 5 degrees per frame
        rotationAngle += 1.0f;
        // Wrap around if it exceeds 360 degrees
        if (rotationAngle >= 360.0f) { rotationAngle -= 360.0f; }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GLCall(glDeleteProgram(shader));

    glfwTerminate();

    return 0;
}
