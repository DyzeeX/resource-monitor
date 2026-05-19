#pragma once

#include "Component.hpp"

struct RAMFileStat {
    long long total = 0, available = 0; 
};

class RAMComponent : public Component<RAMFileStat> {
public:
    RAMComponent(const char* path);
    RAMFileStat ReadStat() override;
    double GetRAMPercentage(const double avlb, const double ttl);
};