#pragma once
#include "imgui.h"

class Circle
{
public:
    ImVec2 position;
    float  radius;
    ImU32  color;

    Circle(ImVec2 position, float radius, ImU32 color = IM_COL32(80, 160, 255, 255))
        : position(position), radius(radius), color(color) {}

    void Draw() const
    {
        ImGui::GetBackgroundDrawList()->AddCircleFilled(position, radius, color);
    }
};
