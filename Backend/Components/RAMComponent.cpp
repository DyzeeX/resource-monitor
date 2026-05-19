#include "RAMComponent.hpp"

#include <fstream>

RAMComponent::RAMComponent(const char* path) : Component(path) {}

void RAMComponent::Update() {
    std::ifstream file(target_file_path);

    std::string temp_key, temp_unit;
    long long value;
    while (file >> temp_key >> value >> temp_unit) {
        if (temp_key == "MemTotal:") {
            m_stats.total_kb = value;
        }
        
        if (temp_key == "MemAvailable:") {
            m_stats.available_kb = value;
        }
    }
}