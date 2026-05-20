#pragma once

// Basic class
class Component {
public:
    explicit Component(const char* path) : target_file_path(path) {}
    virtual ~Component() = default;
    
    // Updating info about component
    virtual void Update() = 0;
protected:
    const char* target_file_path;
};