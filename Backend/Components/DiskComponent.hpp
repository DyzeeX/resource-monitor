#pragma once

#include "Component.hpp"

#include <string>
#include <chrono> 


struct DiskFileStat {
    unsigned long long reads_completed;
    unsigned long long sectors_read;
    unsigned long long writes_completed;
    unsigned long long sectors_written;
    unsigned long long io_time_ms;
};

class DiskComponent : public Component {
public:
    explicit DiskComponent(const std::string& device_name = "sda", const char* path = "/proc/diskstats");
    void Update() override;

    double GetReadBytesPerSec() const { return m_read_bytes_per_sec; }
    double GetWriteBytesPerSec() const { return m_write_bytes_per_sec; }
    double GetIOUtilPercent() const { return m_io_util_percent; }

private:
    std::string m_device_name;

    DiskFileStat m_prev{};
    std::chrono::steady_clock::time_point m_prev_time;

    double m_read_bytes_per_sec = 0.0;
    double m_write_bytes_per_sec = 0.0;
    double m_io_util_percent = 0.0;

    static constexpr unsigned long long SECTOR_SIZE = 512;

    DiskFileStat ReadRaw() const;
};