#include <iostream>
#include <thread>
#include <chrono>

#include "Components/CPUComponent.hpp"

int main() { 
    CPUComponent cpu("/proc/stat");
    auto previous = cpu.ReadStat();

    for(int i = 1; i <= 100; i++){

        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        auto current = cpu.ReadStat();

        std::cout << "CPU Usage: " << cpu.GetCPUPercentage(previous, current) << "%" << std::endl;  
        
        previous = current; 
    }
    return 0;
}