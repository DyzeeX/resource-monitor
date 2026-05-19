#include <iostream>
#include <thread>
#include <chrono>

#include "Components/RAMComponent.hpp"

int main() { 
    
    RAMComponent ram("/proc/meminfo");
    std::cout << ram.ReadStat().total << ' ' << ram.ReadStat().available;
    return 0;
}