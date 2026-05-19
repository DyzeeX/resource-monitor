#include "RAMComponent.hpp"

#include <fstream>
#include <string>

RAMComponent::RAMComponent(const char* path) : Component(path) {}

RAMFileStat RAMComponent::ReadStat() {
    std::ifstream file(target_file_path);

    RAMFileStat res{};
    std::string temp_key, temp_unit;
    long long value;
    while(file >> temp_key >> value >> temp_unit) {
        if (temp_key == "MemTotal:") {
            res.total = value;
        }

        if (temp_key == "MemAvailable:") {
            res.available = value;
        }
    }
    return res;
}