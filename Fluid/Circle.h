#pragma once
#include "imgui.h"
#include "movableShape.h"
#include "ourMath.h"
class Circle : public movableShape
{
public:
    float  radius;
    ImU32  color;

    Circle(Vector2f position, Vector2f speed, Vector2f acceleration, float radius, ImU32 color = IM_COL32(80, 160, 255, 255))
        : movableShape(position, speed, acceleration), radius(radius), color(color) { }

    void render()  const override 
    {
        ImGui::GetBackgroundDrawList()->AddCircleFilled(position, radius, color);
    }
};