#include "CPUComponent.hpp"

#include <iostream>
#include <fstream>
#include <string>

CPUComponent::CPUComponent(const char* path) : Component(path) {}

CPUFileStat CPUComponent::ReadStat() {
    std::ifstream file(target_file_path);

    std::string temp;
    CPUFileStat res{};

    // struct CPUFileStat {
    //     long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    // };
    
    if (file >> temp >> res.user >> res.nice 
             >> res.system >> res.idle >> res.iowait 
             >> res.irq >> res.softirq >> res.steal 
             >> res.guest >> res.guest_nice) {
        return res;
    }
    return {};

}