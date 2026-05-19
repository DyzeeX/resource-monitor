#include <iostream>
#include <thread>
#include <chrono>

#include "Components/ProcessComponent.hpp"

int main() { 
    ProcessComponent pc("/proc");

    auto res = pc.ReadStat();

    for (const auto& p : res) {
        std::cout << p.PID << "\t" << p.user << "\t\t" << p.state << "\t" << p.name << std::endl;
    }
    return 0;
}