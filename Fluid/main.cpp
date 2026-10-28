#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "Circle.h"
#include "Physics.h"
#include "PhysicsDod.h"
#include "ourMath.h"
#include "globalVariables.h"
#include "movableShape.h"

int main()
{
    // Fereastra fullscreen
    glfwInit();
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* screen = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(screen->width, screen->height, "Fluid", monitor, nullptr);
    globalVariables::screenHeight = screen->height;
    globalVariables::screenWidth = screen->width;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // vsync

    // ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    //Physics meneger;
    PhysicsDod meneger;
    // Game loop
    int nrDeCercuri=0;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Generare cercuri");
        ImGui::Text("fps:%.1f", ImGui::GetIO().Framerate);
        if (ImGui::SliderInt("Nr de cercuri", &nrDeCercuri, 0, 200000))
            meneger.changeNumberOfCircles(nrDeCercuri);
        ImGui::End();
        

        meneger.update();

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
