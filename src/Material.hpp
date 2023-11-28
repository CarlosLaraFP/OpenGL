#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Material
{
private:
	Shader m_Shader;
	Texture m_Texture;
	float m_RotationAngle { 0.0f }; // Initialize rotation angle
	float m_RotationIncrement;
	float m_Red { 0.0f }; // Initialize color
	float m_ColorIncrement;
	glm::mat4 m_ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	void BindTexture();
	void IncrementRotationAngle();
	void IncrementColor();

public:
	Material(const ShaderPaths filePaths, const std::string& texturePath, float colorIncrement, float rotationIncrement);

	void Bind();
	void UpdateProjectionMatrix();
	void SetModelMatrix(glm::mat4 modelMatrix) { m_ModelMatrix = modelMatrix; }

	inline glm::mat4 GetModelMatrix() const { return m_ModelMatrix; }
};
