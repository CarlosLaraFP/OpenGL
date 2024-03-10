#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// TODO: Replace with a safer approach
int g_WindowWidth = 640;
int g_WindowHeight = 480;
bool g_WindowResized = true;
float g_CameraOffsetX = 0.0f; // Horizontal camera offset
float g_CameraOffsetY = 0.0f; // Vertical camera offset

// (px, py), (tx, ty), (r, g, b, a), (t)
static std::array<Vertex, 4> CreateQuad(float x, float y, float t)
{
    Vertex v0;
    v0.Position = { -0.5f + x, -0.5f + y };
    v0.TextureCoordinates = { 0.0f, 0.0f };
    v0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v0.TextureID = t;

    Vertex v1;
    v1.Position = { 0.5f + x, -0.5f + y };
    v1.TextureCoordinates = { 1.0f, 0.0f };
    v1.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v1.TextureID = t;

    Vertex v2;
    v2.Position = { 0.5f + x, 0.5f + y };
    v2.TextureCoordinates = { 1.0f, 1.0f };
    v2.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v2.TextureID = t;

    Vertex v3;
    v3.Position = { -0.5f + x, 0.5f + y };
    v3.TextureCoordinates = { 0.0f, 1.0f };
    v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v3.TextureID = t;

    return { v0, v1, v2, v3 };
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
            ImGui::Begin("ImGui");

            for (auto i = 0; i < translations.size(); ++i)
            {
                std::string label = "Translation " + std::to_string(i);
                ImGui::SliderFloat3(label.c_str(), glm::value_ptr(translations[i]), -1.0f, 1.0f);
            }
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

        // TODO: Load mesh from file
        auto q0 = CreateQuad(-0.5f, 0.0f, 0.0f);
        auto q1 = CreateQuad(0.5f, 0.0f, 1.0f);

        std::vector<Vertex> vertices;
        vertices.reserve(8);
        vertices.insert(vertices.end(), std::begin(q0), std::end(q0));
        vertices.insert(vertices.end(), std::begin(q1), std::end(q1));

        square.SetVertexData(std::move(vertices));

        std::vector<VertexAttribute> vertexAttributes;
        vertexAttributes.push_back({ 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position) });
        vertexAttributes.push_back({ 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TextureCoordinates) });
        vertexAttributes.push_back({ 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, Color) });
        vertexAttributes.push_back({ 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, TextureID) });

        square.SetVertexAttributes(std::move(vertexAttributes));

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
