#include "Material.hpp"
#include "Context.hpp"
#include "Globals.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

Material::Material(const ShaderPaths filePaths, const std::string& texturePathA, const std::string& texturePathB, float colorIncrement, float rotationIncrement)
    : m_Shader { filePaths }, m_TextureA { texturePathA }, m_TextureB { texturePathB },
      m_ColorIncrement{ colorIncrement }, m_RotationIncrement{ rotationIncrement } 
{
    BindTextures();
}

void Material::Bind()
{
    // Shader only needs to be bound once (in ctor) because it's the only one used throughout.
    UpdateModelMatrix();
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

void Material::BindTextures()
{
    m_TextureA.Bind(0);
    //m_Shader.SetUniform1i("u_TextureA", m_TextureA.GetSlot());

    m_TextureB.Bind(1);
    //m_Shader.SetUniform1i("u_TextureB", m_TextureB.GetSlot());

    int samplers[2] = { 0, 1 };

    m_Shader.SetUniform1iv("u_Textures", 2, samplers);
}

// Translations, Rotations, Scaling
void Material::UpdateModelMatrix()
{
    m_TranslationMatrix = glm::translate(glm::mat4(1.0f), m_Translation);
    m_Shader.SetUniformMatrix4fv("u_Model", m_TranslationMatrix);
    m_Shader.SetUniform1f("u_Rotation", m_RotationAngle);
    IncrementRotationAngle();
}

// Camera transformation
void Material::UpdateViewMatrix()
{
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(g_CameraOffsetX, g_CameraOffsetY, 0.0f));
    m_Shader.SetUniformMatrix4fv("u_View", viewMatrix);
}

// This should be called whenever the window is resized (new clip space).
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
