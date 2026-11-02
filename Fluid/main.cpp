#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "Physics.h"
#include "PhysicsDod.h"
#include "ourMath.h"
#include "globalVariables.h"
#include "ProcessInfo.h"

#include <iostream>



int main()
{
    // Fereastra fullscreen
    glfwInit();
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* screen = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(screen->width, screen->height, "Fluid", nullptr, nullptr);
    globalVariables::screenHeight = screen->height;
    globalVariables::screenWidth = screen->width;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // vsync

    // GLAD loading
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Nu s-au putut incarca functiile OpenGL (glad)\n";
        return -1;
    }
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);   


    // ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    { // blocul acesta distruge obiectele inainte de glfwTerminate() (care da sigsev daca nu-s distruse beforehand)

        Shader shader("shader.vs", "shader.fs");
        shader.use();
        shader.setFloat("raza", globalVariables::circleRadius);
        shader.setVec2("marimeEcran", globalVariables::screenWidth, globalVariables::screenHeight);

        ProcessInfo proc_info;

        Physics meneger;
        // PhysicsDod meneger;

        int nrDeCercuri = 0; bool show_graph_cpu = false, show_graph_ram = false;

        // Game loop
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("Generare cercuri");

            static int nr_cpu = 90, offset_cpu = 0;
            static float cpu_times[90] = {}, refresh_cpu = 0.0f, last_cpu = 0.0f;

            refresh_cpu += ImGui::GetIO().DeltaTime;
            if (refresh_cpu >= 0.1f) {
                float curr_cpu = proc_info.GetCpuUsage(); if (curr_cpu >= 0.0f) { last_cpu = curr_cpu; }
                refresh_cpu = 0.0f;
                cpu_times[offset_cpu] = last_cpu;
                offset_cpu = (offset_cpu + 1) % nr_cpu;
            }

            ImGui::Text("CPU Usage: %.2f %%", last_cpu);
            ImGui::Checkbox("Show CPU Graph?", &show_graph_cpu);

            if (show_graph_cpu) {
                ImGui::PlotLines(
                "##CPU", cpu_times, nr_cpu, offset_cpu, nullptr, 0.0f, 100.0f, Vector2f(0, 80.0f)
                );
            }

            ImGui::Separator();

            static int nr_ram = 90, offset_ram = 0;
            static float ram_usages[90] = {}, refresh_ram = 0.0f, last_ram = 0.0f;

            refresh_ram += ImGui::GetIO().DeltaTime;
            if (refresh_ram >= 0.1f) {
                float curr_ram = proc_info.GetMemoryUsage() / 1048576.0f;
                if (curr_ram >= 0.0f) { last_ram = curr_ram; }

                refresh_ram = 0.0f;
                ram_usages[offset_ram] = last_ram - 100.0f;
                offset_ram = (offset_ram + 1) % nr_ram;
            }

            ImGui::Text("RAM Usage: %.2f MB", last_ram);
            ImGui::Checkbox("Show RAM Graph?", &show_graph_ram);

            if (show_graph_ram) {
                ImGui::PlotLines(
                "##RAM", ram_usages, nr_ram, offset_ram, nullptr, 0.0f, 100.0f, Vector2f(0, 80.0f)
                );
            }

            ImGui::Separator();

            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

            ImGui::Separator();

            // FILE *file = fopen("/proc/self/status", "r");
            // char line[128];
            //
            // while (fgets(line, 128, file) != nullptr)
            // {
            //     if (strncmp(line, "VmRSS:", 6) == 0)
            //     {
            //         ImGui::Text("/status/ ram usage: \"%s\"", line);
            //         break;
            //     }
            // }
            // fclose(file);

            if (ImGui::SliderInt("Nr de cercuri", &nrDeCercuri, 0, 200000))
                meneger.changeNumberOfCircles(nrDeCercuri);

            ImGui::Separator();
            ImGui::SliderFloat("Speed multiplier", &globalVariables::speedMultiplier, 0.1f, 10.0f);
            ImGui::End();



            meneger.update();

            ImGui::Render();
            glClear(GL_COLOR_BUFFER_BIT);
            meneger.render(shader);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
        }

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
