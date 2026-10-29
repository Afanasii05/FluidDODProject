cmake_minimum_required(VERSION 3.12)
project(Fluid)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(vendor/sys_info)

# ImGui directories
include_directories(
    Fluid
    vendor/imgui
    vendor/imgui/backends
    vendor/glad/include
    vendor/sys_info/core/include/
    vendor/glm
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
    Fluid/Shader.cpp
    vendor/glad/src/glad.c
)

add_executable(Fluid ${SOURCES})

# Packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(Fluid PRIVATE glfw OpenGL::GL dl pthread sys_core)

# get shaders so opengl runs
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Fluid/shader.vs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Fluid/shader.fs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})