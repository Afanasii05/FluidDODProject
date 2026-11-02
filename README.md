# Fluid

Aplicatie de simulare de comportament a 200.000 de obiecte (cercuri). Obiectele se genereaza printr-un slider si o sa se miste in mod aleator pe fereastra deschisa, colizionand intre ele si cu marginea ferestrei. Se poate face un switch intre OOP si DOD pentru logica obiectelor.

## Rulare Windows
1. `git clone ...`
2. Deschide `Fluid.sln` in Visual Studio 2022
3. Platforma **x64**, apoi **F5**

## Rulare Linux
1. `git clone ...`
2. Ruleaza comenzile in terminal:
`
    mkdir build
    cd build
    cmake ..
    make
    ./Fluid
`
3. Posibil sa ceara dependintele de cmake. Rulati comanda urmatoare: `sudo apt update && sudo apt install cmake build-essential`

Pentru a iesi din aplicatie apasati tasta ESC.

## Structura
```
Fluid.sln
Fluid/        main.cpp (game loop), Circle.h
vendor/imgui  Dear ImGui 1.93.0
vendor/glfw   GLFW 3.5.1 (header + glfw3.lib, x64)
```
Nu trebuie instalat nimic in plus — tot ce trebuie e in `vendor/`.
