#pragma once

#include "Components/CPUComponent.hpp"
#include "Components/ProcessComponent.hpp"
#include "Components/RAMComponent.hpp"

class ResourceMonitor {
public:
    ResourceMonitor();

    void Update();

    const CPUComponent& GetCPU() const { return m_cpu; }
    const RAMComponent& GetRAM() const { return m_ram; }
    const ProcessComponent& GetProc() const { return m_proc; }
private:
    CPUComponent m_cpu;
    RAMComponent m_ram;
    ProcessComponent m_proc;
};