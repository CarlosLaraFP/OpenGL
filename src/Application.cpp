#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <cmath>
/*
    GLEW is used to interface with this machine's GPU drivers to access NVIDIA's OpenGL code implementation.
    It simplifies the process of accessing advanced features and extensions in OpenGL that are not included in the standard OpenGL distribution.
    Must be included before GLFW.
*/
#include <GL/glew.h>
/*
    GLFW is essentially a cross-platform library that provides an abstraction over the operating-system-specific APIs for creating windows, 
    handling input, event polling, time measurement, and managing OpenGL contexts. This abstraction allows OpenGL developers to write code 
    that is largely platform-independent, avoiding the need to directly interact with the low-level, platform-specific APIs.
*/
#include <GLFW/glfw3.h>
/*
    Use docs.gl for OpenGL documentation and in-depth understanding. Its mastery is key.
    Avoid creating functions and variables with OpenGL types; use C++
*/
#include "Context.hpp"
#include "Renderer.hpp"
#include "Square.hpp"
#include "Globals.hpp"

#include "glm/gtc/type_ptr.hpp"
#include <glm/glm.hpp> // Include the main GLM header
#include <glm/gtc/matrix_transform.hpp> // Include for transformation functions

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// TODO: Replace with a safer approach
int g_WindowWidth = 640;
int g_WindowHeight = 480;
bool g_WindowResized = true;
float g_CameraOffsetX = 0.0f; // Horizontal camera offset
float g_CameraOffsetY = 0.0f; // Vertical camera offset

static unsigned int vertexCount = 0;

// (px, py), (tx, ty), (r, g, b, a), (t)
static std::array<Vertex, 4> CreateQuad(float x, float y, float rotationDegrees, float t) {
    float rad = rotationDegrees * M_PI / 180.0f; // Convert degrees to radians
    // Rotation matrix components
    float cosRad = cos(rad);
    float sinRad = sin(rad);

    // Define the quad centered at the origin
    std::array<Vertex, 4> vertices = { {
        {{-0.5f, -0.5f}, {0.0f, 0.0f}, {0.18f, 0.6f, 0.96f, 1.0f}, t},
        {{ 0.5f, -0.5f}, {1.0f, 0.0f}, {0.18f, 0.6f, 0.96f, 1.0f}, t},
        {{ 0.5f,  0.5f}, {1.0f, 1.0f}, {0.18f, 0.6f, 0.96f, 1.0f}, t},
        {{-0.5f,  0.5f}, {0.0f, 1.0f}, {0.18f, 0.6f, 0.96f, 1.0f}, t}
    } };

    // Apply rotation and translation to each vertex
    for (Vertex& v : vertices) {
        // Rotate around the origin
        float rotatedX = v.Position.x * cosRad - v.Position.y * sinRad;
        float rotatedY = v.Position.x * sinRad + v.Position.y * cosRad;

        // Translate the vertex
        v.Position.x = rotatedX + x;
        v.Position.y = rotatedY + y;
    }

    vertexCount += 4;

    return vertices;
}

int main(void)
{
    Context context;

    Square square;

    /*
        Useful because we have multiple objects in our scene that share the same geometry but are positioned differently. 
        We use an array or vector of transformation matrices to render multiple instances of an object at different positions.
        We can choose an array since the size is known at compile time (otherwise we would use a vector).
    */
    std::vector<glm::vec3> translations;
    translations.emplace_back(-0.50f, 0.0f, 0.0f);
    translations.emplace_back(0.50f, 0.0f, 0.0f);

    Renderer renderer;


    const char* glsl_version = "#version 430 core";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(context.GetWindow(), true);  // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(context.GetWindow()))
    {
        /* Poll for and process events */
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        
        {
            ImGui::Begin("Controls");
            /*
            for (auto i = 0; i < translations.size(); ++i)
            {
                std::string label = "Translation " + std::to_string(i);
                ImGui::SliderFloat3(label.c_str(), glm::value_ptr(translations[i]), -1.0f, 1.0f);
            }
            */
            ImGui::DragFloat2("Texture A Position", square.m_Position, 0.1f);
            ImGui::DragFloat("Texture A Rotation", &square.m_RotationAngle, 1.0f, 0.0f, 360.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        /* Render here */
        renderer.Clear();

        context.HandleCameraMovement();

        // Replaced with batch rendering (single draw call) by using a single (dynamic) vertex buffer
        /*for (const auto& translation : translations)
        {
            square.SetTranslationVector(translation);
            renderer.Draw(square);
        }*/
        // (px, py), (tx, ty), (r, g, b, a), (t)
        /*float vertices[] =
        {
            // First square (left)
            -0.5f - 0.5f, -0.5f, 0.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, // 0
             0.5f - 0.5f, -0.5f, 1.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, // 1
             0.5f - 0.5f,  0.5f, 1.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, // 2
            -0.5f - 0.5f,  0.5f, 0.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, // 3

            // Second square (right)
            -0.5f + 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, // 4
             0.5f + 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, // 5
             0.5f + 0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, // 6
            -0.5f + 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f  // 7
        };*/

        vertexCount = 0;

        // TODO: Load mesh from OBJ Blender file
        auto q0 = CreateQuad(square.m_Position[0], square.m_Position[1], square.m_RotationAngle, 0.0f);
        //square.IncrementRotationAngle();
        auto q1 = CreateQuad(0.5f, 0.0f, 0.0f, 1.0f);

        std::vector<Vertex> vertices;
        vertices.reserve(vertexCount);
        vertices.insert(vertices.end(), std::begin(q0), std::end(q0));
        vertices.insert(vertices.end(), std::begin(q1), std::end(q1));

        square.SetVertexData(std::move(vertices));

        std::vector<VertexAttribute> vertexAttributes;
        vertexAttributes.push_back({ 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position) });
        vertexAttributes.push_back({ 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TextureCoordinates) });
        vertexAttributes.push_back({ 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, Color) });
        vertexAttributes.push_back({ 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, TextureID) });

        square.SetVertexAttributes(std::move(vertexAttributes));

        std::vector<unsigned int> indices;
        indices.reserve(vertexCount / 4 * 6); // works for quads-only geometry

        for (size_t i = 0; i < vertexCount / 4; ++i)
        {
            // Add indices for the first triangle of the quad
            indices.push_back(0 + i * 4);
            indices.push_back(1 + i * 4);
            indices.push_back(2 + i * 4);
            // Add indices for the second triangle of the quad
            indices.push_back(2 + i * 4);
            indices.push_back(3 + i * 4);
            indices.push_back(0 + i * 4);
        }

        square.SetIndexBuffer(std::move(indices));

        renderer.Draw(square);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(context.GetWindow());
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;

    // Destructors called in reverse order of creation at the end of this scope.
}
