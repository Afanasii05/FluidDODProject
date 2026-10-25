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
     * @return float The normalized multiplier to apply to the movement vector.
     */
    static float normalized_movement_mult(float cur_dist, float total_dist, float frict_fail = 0) {
        return std::max(cur_dist / total_dist, frict_fail);
    }

    static bool is_negative(int val) { return val < 0; }
    static bool is_negative(float val) { return val < 0; }
    static bool is_negative(double val) { return val < 0; }
};

#endif //FLUID_FUNCS_H
