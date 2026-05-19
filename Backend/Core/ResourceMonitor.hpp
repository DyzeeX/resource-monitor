#pragma once

#include "Components/CPUComponent.hpp"
#include "Components/ProcessComponent.hpp"
#include "Components/RAMComponent.hpp"

class ResourceMonitor {
public:
    ResourceMonitor();

    CPUComponent& GetCPU();
    RAMComponent& GetRAM();
    ProcessComponent& GetProc();
private:
    CPUComponent m_cpu;
    RAMComponent m_ram;
    ProcessComponent m_proc;
};