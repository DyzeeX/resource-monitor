#pragma once

#include "Component.hpp"

// 2 fields and some helping us functions 
struct RAMStats {
    long long total_kb = 0, available_kb = 0;

    long long used_mb()  const { return (total_kb - available_kb) / 1024; }

    long long total_mb() const { return total_kb / 1024; }

    double usage_percent() const {
        if (total_kb == 0) return 0.0;
        return (double)(total_kb - available_kb) / total_kb * 100.0;
    }
};

class RAMComponent : public Component {
public:
    explicit RAMComponent(const char* path = "/proc/meminfo");
    
    // Get 2 values: MemTotal and MemAvailable
    void Update() override;
    const RAMStats& GetStats() const { return m_stats; }

private:
    RAMStats m_stats{};
};