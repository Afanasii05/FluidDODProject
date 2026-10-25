#pragma once
#include "imgui.h"
#include "Object.h"

class Circle : public Object {
public:
    float radius;

    Circle(ImVec2 position, float radius, ImU32 color = IM_COL32(80, 160, 255, 255))
        : Object(position, color), radius(radius) {}

    // default functions
    void Draw() const { ImGui::GetBackgroundDrawList()->AddCircleFilled(pos, radius, color); }
    void Update()  {
        if (moving) {
            if (movDist <= 0.0f) {
                pos = endPos; movDist = 0; movVec = { 0, 0 }; speed = 0; moving = false;
            } else {
                float frict = Funcs::normalized_movement_mult(movDist, totalDist, 0.01f);
                float delta_spd = ImGui::GetIO().DeltaTime * speed * frict;
                pos += movVec * delta_spd; movDist -= delta_spd;
            }
        }
        Draw();
    }
};
