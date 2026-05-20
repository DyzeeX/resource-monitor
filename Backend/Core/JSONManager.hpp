#pragma once

#include <nlohmann/json.hpp>

class ResourceMonitor;

class JSONManager {
public:
    JSONManager() = delete;    

    static nlohmann::json Convert(const ResourceMonitor& rm);
    static std::string ConvertToString(const ResourceMonitor& rm);
};