#include "CPUComponent.hpp"

#include <iostream>
#include <fstream>
#include <string>

CPUComponent::CPUComponent(const char* path) : Component(path) {}

CPUFileStat CPUComponent::ReadStat() {
    std::ifstream file(target_file_path);

    std::string temp;
    CPUFileStat res{};
    
    if (file >> temp >> res.user >> res.nice 
             >> res.system >> res.idle >> res.iowait 
             >> res.irq >> res.softirq >> res.steal 
             >> res.guest >> res.guest_nice) {
        return res;
    }
    return {};

}

    double CPUComponent::GetCPUPercentage(const CPUFileStat& previous, const CPUFileStat& current) {

        auto totalPrev = previous.user + previous.nice + previous.system 
                         + previous.idle + previous.iowait + previous.irq
                         + previous.softirq + previous.steal + previous.guest
                         + previous.guest_nice;
        auto totalCurr = current.user + current.nice + current.system 
                         + current.idle + current.iowait + current.irq
                         + current.softirq + current.steal + current.guest
                         + current.guest_nice;

        auto busyPrev = totalPrev - previous.idle - previous.iowait;
        auto busyCurr = totalCurr - current.idle - current.iowait;

        auto dTotal = totalCurr - totalPrev;
        if (dTotal == 0) return 0.0;

        return (double)(busyCurr - busyPrev) / dTotal * 100.0;
    }