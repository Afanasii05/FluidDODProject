# Fluid

Fereastra fullscreen cu un cerc (Dear ImGui + GLFW + OpenGL).

## Rulare
1. `git clone ...`
2. Deschide `Fluid.sln` in Visual Studio 2022
3. Platforma **x64**, apoi **F5**

ESC = iesire.

## Structura
```
Fluid.sln
Fluid/        main.cpp (game loop), Circle.h
vendor/imgui  Dear ImGui 1.93.0
vendor/glfw   GLFW 3.5.1 (header + glfw3.lib, x64)
```
Nu trebuie instalat nimic in plus — tot ce trebuie e in `vendor/`.
