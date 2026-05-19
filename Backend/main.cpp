#include <iostream>
#include <memory>

#include "Components/CPUComponent.hpp"

int main() {
    CPUComponent cpu("/proc/stat");
    auto a = cpu.ReadStat();
    std::cout << a.user << ' ' << a.nice << ' ' << a.system << ' ' << a.idle << ' ' << a.iowait
     << ' ' << a.irq << ' ' << a.softirq << ' ' << a.steal << ' ' << a.guest << ' ' << a.guest_nice;
    return 0;
}