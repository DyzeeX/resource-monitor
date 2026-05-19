#include "CPUComponent.hpp"

#include <fstream>

CPUComponent::CPUComponent(const char* path) : Component(path) {
    m_prev = ReadRaw(); 
}

void CPUComponent::Update() {
    CPUFileStat curr = ReadRaw();

    m_usage_percent = CalcPercent(m_prev, curr);
    m_prev = curr;
}

CPUFileStat CPUComponent::ReadRaw() const {
    std::ifstream file(target_file_path);

    std::string temp;
    CPUFileStat res{};

    file >> temp >> res.user >> res.nice 
         >> res.system >> res.idle >> res.iowait
         >> res.irq >> res.softirq >> res.steal
         >> res.guest >> res.guest_nice;
}

double CPUComponent::CalcPercent(const CPUFileStat& prev, const CPUFileStat& curr) {
    auto sum = [](const CPUFileStat& s) {
        return s.user + s.nice + s.system + s.idle + s.iowait + s.irq + s.softirq + s.steal;
    };

    auto dTotal = sum(curr) - sum(prev);

    if (dTotal == 0) {
        return 0.0;
    }

    auto dBusy = (sum(curr) - curr.idle - curr.iowait) - (sum(prev) - prev.idle - prev.iowait);
    return (double)dBusy / dTotal * 100.0;
}