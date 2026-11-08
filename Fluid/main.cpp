#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "PhysicsOOP.h"
#include "PhysicsDOD.h"
#include "ourMath.h"
#include "globalVariables.h"
#include "ProcessInfo.h"

#include <iostream>

enum PhysicsType { OOP, DOD };

static PhysicsOOP* manager_oop; static PhysicsDOD* manager_dod;
static int nrDeCercuri, cur_physics_engine, last_physics_engine;
static bool show_graph_cpu, show_graph_ram;


static void display_debugWindow(ProcessInfo& proc_info);
static void display_settingsWindow();

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

        manager_oop = nullptr; manager_dod = new PhysicsDOD();
        nrDeCercuri = 0; cur_physics_engine = DOD; last_physics_engine = cur_physics_engine;
        show_graph_cpu = false; show_graph_ram = false;

        // Game loop
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);


            //Coordonate mouse
            double mX, mY;
            glfwGetCursorPos(window, &mX, &mY);
            globalVariables::mouseX = (float)mX;
            globalVariables::mouseY = (float)mY;

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            display_settingsWindow(); // settings for app
            display_debugWindow(proc_info); // debug options

            if (cur_physics_engine == DOD && manager_dod != nullptr) { manager_dod->update();
            } else if (manager_oop != nullptr) { manager_oop->update(); }

            // sincronizeaza slider-ul cu numarul real de bile (scade cand se contopesc)
            if (cur_physics_engine == DOD && manager_dod != nullptr) { nrDeCercuri = (int)manager_dod->getPositions().size(); }
            else if (manager_oop != nullptr) { nrDeCercuri = (int)manager_oop->getShapes().size(); }

            ImGui::Render();
            glClear(GL_COLOR_BUFFER_BIT);

            if (cur_physics_engine == DOD && manager_dod != nullptr) { manager_dod->render(shader); }
            else if (manager_oop != nullptr) { manager_oop->render(shader); }

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
        }

    }

    delete manager_dod; delete manager_oop;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}



static void display_debugWindow(ProcessInfo& proc_info) {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 debugWindowSize(300, 150);

    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x - debugWindowSize.x - 10, viewport->WorkPos.y + 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(debugWindowSize, ImGuiCond_FirstUseEver);

    ImGui::Begin("Debug");

    // FPS Counter
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::Separator();

    // CPU Usage
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
    ImGui::SameLine();
    ImGui::Checkbox("Show CPU Graph?", &show_graph_cpu);

    if (show_graph_cpu) {
        ImGui::PlotLines(
        "##CPU", cpu_times, nr_cpu, offset_cpu, nullptr, 0.0f, 100.0f, Vector2f(0, 80.0f)
        );
    }

    ImGui::Separator();


    // RAM Usage
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
    ImGui::SameLine();
    ImGui::Checkbox("Show RAM Graph?", &show_graph_ram);

    if (show_graph_ram) {
        ImGui::PlotLines(
        "##RAM", ram_usages, nr_ram, offset_ram, nullptr, 0.0f, 100.0f, Vector2f(0, 80.0f)
        );
    }

    ImGui::End();
}

static void display_settingsWindow() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + 10, viewport->WorkPos.y + 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_FirstUseEver);

    ImGui::Begin("Settings");

    // physics type
    ImGui::Text("Physics Type");

    ImGui::RadioButton("OOP", &cur_physics_engine, OOP);
    ImGui::SameLine();
    ImGui::RadioButton("DOD", &cur_physics_engine, DOD);

    if (cur_physics_engine != last_physics_engine) {
        if (cur_physics_engine == OOP) {
            manager_oop = new PhysicsOOP(*manager_dod);
            delete manager_dod; manager_dod = nullptr;
        } else if (cur_physics_engine == DOD) {
            manager_dod = new PhysicsDOD(*manager_oop);
            delete manager_oop; manager_oop = nullptr;
        }
        last_physics_engine = cur_physics_engine;
    }

    ImGui::Separator();

    ImGui::Text("Circle Generation");

    // # of circles
    if (ImGui::SliderInt("Nr. of Circles", &nrDeCercuri, 0, 200000)) {
        if (cur_physics_engine == DOD) { manager_dod->changeNumberOfCircles(nrDeCercuri); }
        else if (manager_oop != nullptr) { manager_oop->changeNumberOfCircles(nrDeCercuri); }
    }

    // spd. multiplier
    ImGui::SliderFloat("Speed Multiplier", &globalVariables::speedMultiplier, 0.1f, 10.0f);

    ImGui::Separator();

    // contopire
    ImGui::Text("Merging");
    ImGui::Checkbox("Merge circles", &globalVariables::mergeEnabled);
    ImGui::BeginDisabled(!globalVariables::mergeEnabled);
    ImGui::SliderFloat("Overlap to merge", &globalVariables::mergeOverlap, 0.0f, 100.0f, "%.0f%%");
    ImGui::EndDisabled();


    //Cursor interaction
    ImGui::Separator();
    ImGui::Text("Cursor Interaction");
    ImGui::Checkbox("Cursor Repel Aura", &globalVariables::cursorRepelEnabled);
    ImGui::BeginDisabled(!globalVariables::cursorRepelEnabled);
    ImGui::SliderFloat("Cursor Radius", &globalVariables::cursorRepelRadius, 20.0f, 300.0f, "%.0f px");
    ImGui::SliderFloat("Repel Force", &globalVariables::repelForce, 1.0f, 100.0f, "%.1f");
    ImGui::EndDisabled();


    ImGui::End();
}