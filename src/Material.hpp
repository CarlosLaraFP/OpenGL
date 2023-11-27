#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include <memory>

class Material
{
private:
	Shader m_Shader;
	Texture m_Texture;
	float m_RotationAngle { 0.0f }; // Initialize rotation angle
	float m_RotationIncrement;
	float m_Red { 0.0f }; // Initialize color
	float m_ColorIncrement;
	glm::mat4 m_ModelMatrix;
	
	void BindTexture();
	void IncrementRotationAngle();
	void IncrementColor();

public:
	// TODO: Pass custom struct MaterialSpec
	Material(Shader&& shader, const std::string& texturePath, glm::mat4 modelMatrix, float colorIncrement, float rotationIncrement);

	void Bind();
	void UpdateProjectionMatrix();
};
