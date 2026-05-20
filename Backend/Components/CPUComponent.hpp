#pragma once

#include "Component.hpp"

// Fields in order like /proc/stat
struct CPUFileStat {
    long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
};

class CPUComponent : public Component {
public:
    explicit CPUComponent(const char* path = "/proc/stat");
    void Update() override; 
    double GetUsagePercent() const { return m_usage_percent; }

private:
    CPUFileStat m_prev{};
    double m_usage_percent = 0.0;

    // Read all cpu info from /proc/stat  
    CPUFileStat ReadRaw() const;
    static double CalcPercent(const CPUFileStat& previous, const CPUFileStat& current);
};