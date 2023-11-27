#include "Material.hpp"
#include "Context.hpp"
#include "Globals.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Material::Material(Shader&& shader, const std::string& texturePath, glm::mat4 modelMatrix, float colorIncrement, float rotationIncrement)
    : m_Shader{ std::move(shader) }, m_Texture{ texturePath }, m_ModelMatrix { modelMatrix },
      m_ColorIncrement{ colorIncrement }, m_RotationIncrement{ rotationIncrement } 
{
    BindTexture();
}

void Material::Bind()
{
    // m_Shader.Bind() does not need to be called every frame because it's only one and the same (called once in Shader constructor)
    m_Shader.SetUniform1f("u_Rotation", m_RotationAngle);
    m_Shader.SetUniformMatrix4fv("u_Model", m_ModelMatrix);
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(g_CameraOffsetX, g_CameraOffsetY, 0.0f));
    m_Shader.SetUniformMatrix4fv("u_View", viewMatrix);

    IncrementRotationAngle();
    UpdateProjectionMatrix();
}

void Material::BindTexture()
{
    m_Texture.Bind();
    m_Shader.SetUniform1i("u_Texture", m_Texture.GetSlot());
}

// This should be called whenever the window is resized.
void Material::UpdateProjectionMatrix()
{
    if (g_WindowResized)
    {
        float aspectRatio = static_cast<float>(g_WindowWidth) / static_cast<float>(g_WindowHeight);
        float scale = 1.0f; // Smaller values zoom in, larger values zoom out
        // Convert into -1 to 1 NDC space (if left and right are -2 and +2, 0 is the center)
        auto projectionMatrix = glm::ortho(-aspectRatio * scale, aspectRatio * scale, -scale, scale, -1.0f, 1.0f);
        m_Shader.SetUniformMatrix4fv("u_ProjectionMatrix", projectionMatrix);
        g_WindowResized = false;
    }
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
