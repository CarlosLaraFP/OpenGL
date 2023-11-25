#include "Material.hpp"

Material::Material(Shader&& shader, const std::string& texturePath, float colorIncrement, float rotationIncrement)
    : m_Shader{ std::move(shader) }, m_Texture{ texturePath },
      m_ColorIncrement{ colorIncrement }, m_RotationIncrement{ rotationIncrement } 
{
    BindTexture();
}

void Material::Bind()
{
    // m_Shader.Bind() does not need to be called every frame because it's only one and the same (called once in Shader constructor)
    m_Shader.SetUniform1f("u_Rotation", m_RotationAngle);
    //m_Shader.SetUniform4f("u_Color", m_Red, 0.3f, 0.8f, 1.0f);

    IncrementRotationAngle();
    //IncrementColor();
}

void Material::BindTexture()
{
    m_Texture.Bind();
    m_Shader.SetUniform1i("u_Texture", m_Texture.GetSlot());
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
