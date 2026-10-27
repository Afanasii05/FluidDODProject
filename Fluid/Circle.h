#pragma once
#include "imgui.h"
#include "movableShape.h"
#include "ourMath.h"
class Circle : public movableShape
{
public:
    float  radius;
    ImU32  color;

    Circle(ourMath::Vector2f position, float radius, ImU32 color = IM_COL32(80, 160, 255, 255))
        : movableShape(position), radius(radius), color(color) { }

    void render()  const override 
    {
        ImGui::GetBackgroundDrawList()->AddCircleFilled(position, radius, color);
    }
};
