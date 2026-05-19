#pragma once

#include <vector>
#include <string>

#include "Component.hpp"

struct ProcessFileStat {
    int PID;
    std::string name;
    std::string user;
    std::string state;
};


class ProcessComponent : public Component<std::vector<ProcessFileStat>> {
public:
    ProcessComponent(const char* path);
    std::vector<ProcessFileStat> ReadStat() override;
private:
    ProcessFileStat ReadProcess(int PID);
};