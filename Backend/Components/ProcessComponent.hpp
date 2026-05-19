#pragma once

#include <vector>

#include "Component.hpp"

struct ProcessFileStat {
    
};


class ProcessComponent : public Component<std::vector<ProcessFileStat>> {
public:
    ProcessComponent(const char* path);
    std::vector<ProcessFileStat> ReadStat() override;
private:
    ReadProcess(int PID);
};