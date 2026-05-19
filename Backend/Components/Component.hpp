#pragma once

template<typename T>
class Component {
public:
    virtual T ReadStat() = 0;
    explicit Component(const char* path) : target_file_path(path) {}
    virtual ~Component() = default;
protected:
    const char* target_file_path;
};