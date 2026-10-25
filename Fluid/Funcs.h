//
// Created by pk36 on 25.10.2026.
//

#ifndef FLUID_FUNCS_H
#define FLUID_FUNCS_H
#pragma once

#include <cmath>
#include "imgui.h"

class Funcs {
public:
    /**
     * @brief Calculates the normalized multiplier for a movement vector between a
     *        current position and a desired position, simulating friction.
     *
     * @param cur_pos current position of an object;
     * @param end_pos end point that the object is sliding towards.
     * @param frict_fail the 0.0 - 1.0 percentage of how much speed should be kept
     *                      when reaching the end point (e.g., 0.1 = 10% of speed is kept);
     * @param vert specifies if it calculates y (true) or x (false) movement
     * @return float The normalized multiplier to apply to the movement vector.
     */
    static float normalize_movement_mult(ImVec2 cur_pos, ImVec2 end_pos, float frict_fail = 0.1f, bool vert = false) {
        return (!vert) ? 1.0f + frict_fail - (abs(cur_pos.x - end_pos.x) / end_pos.x) : 1.0f + frict_fail - (abs(cur_pos.y - end_pos.y) / end_pos.y);
    }
};

#endif //FLUID_FUNCS_H
