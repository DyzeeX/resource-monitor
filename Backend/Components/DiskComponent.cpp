#include "DiskComponent.hpp"

#include <fstream>
#include <sstream>
#include <string>

DiskComponent::DiskComponent(const std::string& device_name, const char* path) : Component(path), m_device_name(device_name) {
    m_prev = ReadRaw();
    m_prev_time = std::chrono::steady_clock::now();
}

void DiskComponent::Update() {
    auto now = std::chrono::steady_clock::now();
    auto curr = ReadRaw();

    double dt = std::chrono::duration<double>(now - m_prev_time).count();
    if (dt <= 0.0) {
        return;
    }

    unsigned long long d_sectors_read = curr.sectors_read - m_prev.sectors_read;
    unsigned long long d_sectors_written = curr.sectors_written - m_prev.sectors_written;
    unsigned long long d_io_time_ms = curr.io_time_ms - m_prev.io_time_ms;

    m_read_bytes_per_sec = (d_sectors_read * SECTOR_SIZE) / dt;
    m_write_bytes_per_sec = (d_sectors_written * SECTOR_SIZE) / dt;

    m_io_util_percent = (d_io_time_ms / (dt * 1000.0)) * 100.0;
    if (m_io_util_percent > 100.0) {
        m_io_util_percent = 100.0;
    }

    m_prev = curr;
    m_prev_time = now;
}

DiskFileStat DiskComponent::ReadRaw() const {
    std::ifstream file(target_file_path);
    
    std::string line;
    DiskFileStat temp{};

    while (std::getline(file, line)) {
        std::istringstream ss(line);

        int major, minor;
        std::string name;

        unsigned long long read_merged, read_ms;
        unsigned long long write_merged, write_ms;
        unsigned long long in_flight, weighted_io_ms;

        ss >> major >> minor >> name
           >> temp.reads_completed >> read_merged >> temp.sectors_read >> read_ms
           >> temp.sectors_written >> write_merged >> temp.writes_completed >> write_ms
           >> in_flight >> temp.io_time_ms >> weighted_io_ms;

        if (name != m_device_name) continue;

        return temp;
    }
    return {};
}