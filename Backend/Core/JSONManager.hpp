#pragma once

#include <nlohmann/json.hpp>

class ResourceMonitor;

class JSONManager {
public:
    JSONManager() = delete;    

    nlohmann::json Convert(const ResourceMonitor& rm);
    std::string ConvertToString(const ResourceMonitor& rm);
};