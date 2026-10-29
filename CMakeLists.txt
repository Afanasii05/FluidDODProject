cmake_minimum_required(VERSION 3.12)
project(Fluid)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(vendor/sys_info)

# ImGui directories
include_directories(
    Fluid
    vendor/imgui
    vendor/imgui/backends
    vendor/sys_info/core/include/
)

# Source files
set(SOURCES
    Fluid/main.cpp
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
    Fluid/SystemInfo.h
)

add_executable(Fluid ${SOURCES})

# Packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(Fluid PRIVATE glfw OpenGL::GL dl pthread sys_core)
