#pragma once

#include "Shader.hpp"
#include "Texture.hpp"

class Material
{
private:
	Shader m_Shader;
	float m_RotationAngle { 0.0f }; // Initialize rotation angle
	float m_RotationIncrement;
	float m_Red { 0.0f }; // Initialize color
	float m_ColorIncrement;
	
	void IncrementRotationAngle();
	void IncrementColor();

public:
	Material(const Shader shader, float colorIncrement, float rotationIncrement)
		: m_Shader{ shader }, m_ColorIncrement{ colorIncrement }, m_RotationIncrement{ rotationIncrement } {}

	void Bind();
	void BindShader();
	void BindTexture();
};
