#pragma once

#include "Component.hpp"

#include <string>
#include <vector>

// Fields we need from /proc/<PID>/status
struct ProcessFileStat {
    int PID = 0;
    std::string name;
    std::string state;
    std::string user;
};

class ProcessComponent : public Component {
public:
    explicit ProcessComponent(const char* path = "/proc");

    void Update() override;
    const std::vector<ProcessFileStat>& GetList() const { return m_list; }

private:
    std::vector<ProcessFileStat> m_list;

    // Read every process from /proc and get values
    std::vector<ProcessFileStat> ReadStat() const;
    // Read just one process by it PID
    ProcessFileStat ReadProcess(int pid) const;

    static bool IsNum(const char* str);
};