cmake_minimum_required(VERSION 3.12)
project(Fluid)

set(CMAKE_CXX_STANDARD 17)

# Include directoarele pentru ImGui și fișierele header
include_directories(
    Fluid
    vendor/imgui
    vendor/imgui/backends
)

# Adăugarea fișierelor sursă (inclusiv cele din vendor)
set(SOURCES
    Fluid/main.cpp
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
    Fluid/Funcs.h
)

add_executable(Fluid ${SOURCES})

# Găsirea și linkuirea bibliotecilor OpenGL și GLFW pe Linux
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(Fluid PRIVATE glfw OpenGL::GL dl pthread)