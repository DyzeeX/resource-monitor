#pragma once

#include "Components/CPUComponent.hpp"
#include "Components/ProcessComponent.hpp"
#include "Components/RAMComponent.hpp"

class ResourceMonitor {
public:
    ResourceMonitor();

    void Update();

    CPUComponent& GetCPU() { return m_cpu; }
    RAMComponent& GetRAM() { return m_ram; }
    ProcessComponent& GetProc() { return m_proc; }
private:
    CPUComponent m_cpu;
    RAMComponent m_ram;
    ProcessComponent m_proc;
};