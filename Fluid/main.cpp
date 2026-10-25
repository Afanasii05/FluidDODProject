#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "Circle.h"
#include "Funcs.h"


int main()
{

    // Fereastra fullscreen
    glfwInit();
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* screen = glfwGetVideoMode(monitor);
    ImVec2 screen_size((float)screen->width / 2.0f, (float)screen->height / 2.0f);

    GLFWwindow* window = glfwCreateWindow(screen_size.x, screen_size.y, "Fluid", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    // ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    Circle circle({ screen_size.x / 2.0f, screen_size.y / 2.0f }, 50.0f);

    float mult = 1.0f;

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // update:
        if (circle.position.x >= screen_size.x) { mult = -1.0f; }
        if (circle.position.x <= 0.0f) { mult = 1.0f; }
        circle.position.x += 5.0f * mult * Funcs::normalize_movement_mult(circle.position, screen_size / 2.0f, 0.25f, false);
        circle.Draw();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}
