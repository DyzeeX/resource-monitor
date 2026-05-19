#include "ResourceMonitor.hpp"

ResourceMonitor::ResourceMonitor() : m_cpu(), m_ram(), m_proc() {}

void ResourceMonitor::Update() {
    m_cpu.Update();
    m_ram.Update();
    m_proc.Update();
}