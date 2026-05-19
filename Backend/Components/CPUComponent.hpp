#pragma once

#include "Component.hpp"

struct CPUFileStat {
    long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
};

class CPUComponent : public Component<CPUFileStat> {
public:
    CPUComponent(const char* path);
    CPUFileStat ReadStat() override;
    double GetCPUPercentage(const CPUFileStat& previous, const CPUFileStat& current);
};  