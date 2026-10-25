//
// Created by pk36 on 25.10.2026.
//

#ifndef FLUID_OBJECT_H
#define FLUID_OBJECT_H
#pragma once
#include "imgui.h"
#include "Funcs.h"

class Object {
protected:
    ImVec2 movVec; ImVec2 endPos; float movDist; float totalDist; float speed;
public:
    bool moving = false;
    ImVec2 pos; ImU32 color;

    Object(ImVec2 pos, ImU32 color = IM_COL32(80, 160, 255, 255))
        : pos(pos), color(color), movVec(0.0f, 0.0f), endPos(0.0f, 0.0f) {}

    // getters / setters
    void moveTo(const ImVec2 &end, const ImVec2 &mov, const float &dist, const float &spd) {
        this->endPos = end; this->movVec = mov;
        this->movDist = dist; this->totalDist = dist; this->speed = spd; moving = true;
    }

    // default functions
    void Draw() const;
    void Update();
};

#endif //FLUID_OBJECT_H
