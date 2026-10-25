#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <cmath>
#include "Circle.h"

int main() {
    // Fereastra fullscreen
    glfwInit();
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* screen = glfwGetVideoMode(monitor);
    ImVec2 screen_size((float)screen->width / 2.0f, (float)screen->height / 2.0f);

    GLFWwindow* window = glfwCreateWindow(screen_size.x, screen_size.y, "Fluid_test", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1); // vsync


    // ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // variable definitions
    Circle circle({ screen_size.x / 2.0f, screen_size.y / 2.0f }, 50.0f);
    float mult = 1.0f; double end_x, end_y;

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Control Panel");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Separator();
        ImGui::Text("Moving: %b", circle.moving);
        ImGui::End();

        // update moving position on left click
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            glfwGetCursorPos(window, &end_x, &end_y);

            // determine distance between click and circle and where and how it should go
            ImVec2 dir((float)end_x - circle.pos.x, (float)end_y - circle.pos.y);
            float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (dist > 0.001f) {
                float norm_x = dir.x / dist, norm_y = dir.y / dist, base_spd = 350.0f * dist / 100.0f;
                circle.moveTo({ (float)end_x, (float)end_y }, { norm_x, norm_y }, dist, base_spd);
            }
        }
        circle.Update();

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
