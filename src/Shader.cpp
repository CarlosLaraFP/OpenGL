#include "Shader.hpp"
#include "Renderer.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "glm/gtc/type_ptr.hpp"

Shader::Shader(const ShaderPaths filePaths) : m_FilePaths { filePaths }, m_RendererID { 0 }
{
    m_RendererID = CreateShader(
    { 
        ParseShader(m_FilePaths.VertexShaderPath), 
        ParseShader(m_FilePaths.FragmentShaderPath) 
    });

    this->Bind();
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

Shader::Shader(Shader&& source) noexcept
{
    m_RendererID = source.m_RendererID;
    m_FilePaths = source.m_FilePaths;
    m_UniformLocations = source.m_UniformLocations;
}

Shader& Shader::operator=(Shader&& source) noexcept
{
    if (this == &source) { return *this; }

    m_RendererID = source.m_RendererID;
    m_FilePaths = source.m_FilePaths;
    m_UniformLocations = source.m_UniformLocations;

    return *this;
}

std::string Shader::ParseShader(const std::string filePath)
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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
unsigned int Shader::CreateShader(const ShaderProgramSource shaderSource)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, shaderSource.VertexShaderSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, shaderSource.FragmentShaderSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    // delete intermediate .obj files since program linking is done
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

// Once the shader is created, every uniform gets assigned an ID so that we can reference it (lookup by name).
int Shader::GetUniformLocation(const char* name)
{
    if (m_UniformLocations.find(name) != m_UniformLocations.end())
    {
        return m_UniformLocations[name];
    }

    GLCall(int location = glGetUniformLocation(m_RendererID, name));
    // If a uniform is declared in the shader file, but it's not used, location will be -1
    ASSERT(location != -1);
    m_UniformLocations[name] = location;
    return location;
}

void Shader::SetUniform1i(const char* name, int value)
{
    unsigned int location = GetUniformLocation(name);
    GLCall(glUniform1i(location, value));
}

void Shader::SetUniform1f(const char* name, float value)
{
    unsigned int location = GetUniformLocation(name);
    GLCall(glUniform1f(location, value));
}

void Shader::SetUniform4f(const char* name, float v0, float v1, float v2, float v3)
{
    unsigned int location = GetUniformLocation(name);
    GLCall(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::SetUniformMatrix4fv(const char* name, glm::mat4 matrix)
{
    unsigned int location = GetUniformLocation(name);
    // GLM matrices do not need to be transposed, as they are already in the format OpenGL expects
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix))); // &matrix[0][0]

}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}
