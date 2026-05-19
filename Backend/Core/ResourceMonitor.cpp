#include "ResourceMonitor.hpp"

ResourceMonitor::ResourceMonitor() : m_cpu("/proc/stat"), m_ram("/proc/meminfo"), m_proc("/procc") {}

CPUComponent& ResourceMonitor::GetCPU() { return m_cpu; }
RAMComponent& ResourceMonitor::GetRAM() { return m_ram; }
ProcessComponent& ResourceMonitor::GetProc() { return m_proc; }