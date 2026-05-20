#pragma once

#include "Components/CPUComponent.hpp"
#include "Components/ProcessComponent.hpp"
#include "Components/RAMComponent.hpp"

// Stiore all components in 1 class
class ResourceMonitor {
public:
    ResourceMonitor();

    // Delegate all components Update() methods 
    void Update();

    const CPUComponent& GetCPU() const { return m_cpu; }
    const RAMComponent& GetRAM() const { return m_ram; }
    const ProcessComponent& GetProc() const { return m_proc; }
private:
    CPUComponent m_cpu;
    RAMComponent m_ram;
    ProcessComponent m_proc;
};