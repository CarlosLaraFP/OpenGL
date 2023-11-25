#pragma once

#include <string>
#include <unordered_map>

struct ShaderPaths
{
	std::string VertexShaderPath;
	std::string FragmentShaderPath;
};

struct ShaderProgramSource
{
	std::string VertexShaderSource;
	std::string FragmentShaderSource;
};

class Shader
{
private:
	ShaderPaths m_FilePaths;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocations; // caching for uniforms
	std::string ParseShader(const std::string filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const ShaderProgramSource shaderSource);
	int GetUniformLocation(const char* name);

public:
	Shader(const ShaderPaths filePaths);
	~Shader();
	Shader(Shader&& source) noexcept; // move constructor
	Shader& operator=(Shader&& source) noexcept; // move assignment operator
	Shader(const Shader& source) = delete; // copy constructor
	Shader& operator=(const Shader & source) = delete; // copy assignment operator

	void Bind() const;
	void Unbind() const;

	// Uniforms are used to pass data from the CPU (C++ app) to the GPU (shader program). Uniforms are set per draw call.
	
	void SetUniform1i(const char* name, int value);
	// Rotation animation via vertex shader.
	void SetUniform1f(const char* name, float value);
	// Color animation via fragment shader.
	void SetUniform4f(const char* name, float v0, float v1, float v2, float v3);
};
