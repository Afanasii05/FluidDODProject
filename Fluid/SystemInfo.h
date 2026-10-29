//
// Created by pk36 on 28.10.2026.
//

#ifndef FLUID_SYSTEMINFO_H
#define FLUID_SYSTEMINFO_H

#include <unistd.h>
#include <fstream>
#include <iostream>


class SystemInfo {
    SystemInfo() {}
public:
    static float getMemoryUsage() {
        std::ifstream statm("/proc/self/statm");
        if (statm.is_open()) {
            long size, resident, share, text, lib, data, dt;
            statm >> size >> resident >> share >> text >> lib >> data >> dt;

            long mem_usage = resident * sysconf(_SC_PAGESIZE);
            float mem = (float)mem_usage / 1024.0f / 1024.0f;

            return mem;
        }
        return -1.0f;
    }
};

#endif //FLUID_SYSTEMINFO_H
