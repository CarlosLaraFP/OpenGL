#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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

int main(void)
{
    Context context;

    Square square;

    /*
        Useful because we have multiple objects in our scene that share the same geometry but are positioned differently. 
        We use an array or vector of transformation matrices to render multiple instances of an object at different positions.
        We choose an array since the size is known at compile time (otherwise we would use a vector).
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

        // TODO: Replace with batch rendering (single draw call) by using a single vertex buffer
        for (const auto& translation : translations)
        {
            square.SetTranslationVector(translation);
            renderer.Draw(square);
        }

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
