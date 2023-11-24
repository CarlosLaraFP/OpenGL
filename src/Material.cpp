#include "Material.hpp"

void Material::Bind()
{
    m_Shader.Bind();
    m_Shader.SetUniform1f("u_Rotation", m_RotationAngle);
    m_Shader.SetUniform4f("u_Color", m_Red, 0.3f, 0.8f, 1.0f);

    IncrementRotationAngle();
    IncrementColor();
}

void Material::IncrementRotationAngle() 
{
    // Increase by 5 degrees per frame
    m_RotationAngle += m_RotationIncrement;
    // Wrap around if it exceeds 360 degrees
    if (m_RotationAngle >= 360.0f) 
    { 
        m_RotationAngle -= 360.0f; 
    }
}

void Material::IncrementColor()
{
    m_Red += m_ColorIncrement;

    if (m_Red > 1.0f) 
    { 
        m_ColorIncrement = -0.02f; 
    }
    else if (m_Red < 0.0f) 
    { 
        m_ColorIncrement = 0.02f; 
    }
}
