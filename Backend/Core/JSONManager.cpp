#include "JSONManager.hpp"
#include "ResourceMonitor.hpp"

#include <fstream>

nlohmann::json JSONManager::Convert(const ResourceMonitor& rm) {
    nlohmann::json nj;
    nj["cpu"]["usage"] = rm.GetCPU().GetUsagePercent();

    const auto& ram = rm.GetRAM().GetStats();

    nj["ram"]["total"] = ram.total_mb();
    nj["ram"]["used"] = ram.used_mb();
    nj["ram"]["usage"] = ram.usage_percent();

    nj["processes"] = nlohmann::json::array();

    for(const auto& p : rm.GetProc().GetList()) {
        nj["processes"].push_back({
            {"pid",   p.PID},
            {"name",  p.name},
            {"state", p.state},
            {"user",  p.user}
        });
    }
    const auto& disk = rm.GetDisk();

    nj["disk"]["read"] = disk.GetReadBytesPerSec();
    nj["disk"]["write"] = disk.GetWriteBytesPerSec();
    nj["disk"]["io"] = disk.GetIOUtilPercent();

    return nj;
}

std::string JSONManager::ConvertToString(const ResourceMonitor& rm) {
    return Convert(rm).dump();
}