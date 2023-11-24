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
	const ShaderPaths m_FilePaths;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocations;
	std::string ParseShader(const std::string filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const ShaderProgramSource shaderSource);
	unsigned int GetUniformLocation(const char* name);
	// caching for uniforms

public:
	Shader(const ShaderPaths filePaths);
	~Shader();
	// TODO: Move constructor
	// TODO: Move assignment operator
	// TODO: Copy constructor
	// TODO: Copy assignment operator

	void Bind() const;
	void Unbind() const;

	// Uniforms are used to pass data from the CPU (C++ app) to the GPU (shader program). Uniforms are set per draw call.
	
	// Rotation animation via vertex shader.
	void SetUniform1f(const char* name, float value);
	// Color animation via fragment shader.
	void SetUniform4f(const char* name, float v0, float v1, float v2, float v3);
};
