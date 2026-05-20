#pragma once

#include <nlohmann/json.hpp>

class ResourceMonitor;

class JSONManager {
public:
    JSONManager() = delete;    

    // Get JSON with all metrics
    static nlohmann::json Convert(const ResourceMonitor& rm);
    // Convert JSON format to std::string
    static std::string ConvertToString(const ResourceMonitor& rm);
};