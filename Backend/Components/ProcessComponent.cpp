#include "ProcessComponent.hpp"

#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>

ProcessComponent::ProcessComponent(const char* path) : Component(path) {}

void ProcessComponent::Update() {
    m_list = ReadStat();
}

bool ProcessComponent::IsNum(const char* str) {
    if(*str == '\0') return false;
    while(*str) {
        if (!std::isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

std::vector<ProcessFileStat> ProcessComponent::ReadStat() const {
    std::vector<ProcessFileStat> result{};

    struct dirent* entry;
    DIR* dir;
    dir = opendir(target_file_path);
    if (!dir) return result;

    while ((entry = readdir(dir)) != nullptr) {
        
        if (!IsNum(entry->d_name)) continue;

        std::string full_path = std::string(target_file_path) + '/' + std::string(entry->d_name);

        struct stat st{};
        if (lstat(full_path.c_str(), &st) == -1) {
            continue;
        }

        if (!S_ISDIR(st.st_mode)) {
            continue;
        }

        try {
            int pid = std::stoi(entry->d_name);
            ProcessFileStat stat = ReadProcess(pid);

            if (stat.PID != 0) {
                result.push_back(stat);
            }
        } catch(...) {
            continue;
        }

    }
    closedir(dir);
    return result;
}

ProcessFileStat ProcessComponent::ReadProcess(int pid) const {
    std::string status_path = std::string(target_file_path) + '/' + std::to_string(pid) + "/status";
    std::ifstream file(status_path);

    ProcessFileStat res{};
    if (!file.is_open()) {
        return res;
    }

    res.PID = pid;
    std::string line;
    uid_t uid = 0;
    bool found_name = false, found_state = false, found_uid = false;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;

        iss >> key;

        if (key == "Name:") {
            iss >> res.name;
            found_name = true;
        } else if (key == "State:") {
            iss >> res.state;
            found_state = true;
        } else if (key == "Uid:") {
            iss >> uid;
            found_uid = true;
        }

        if (found_name && found_state && found_uid) break;
    }

    if (!found_name) {
        res.PID = 0;
        return res;
    }

    if (found_uid) {
        struct passwd  pw{};
        struct passwd* pw_result = nullptr;
        std::vector<char> buf(2048);

        if (getpwuid_r(uid, &pw, buf.data(), buf.size(), &pw_result) == 0 && pw_result != nullptr) {
            res.user = pw_result->pw_name;
        } else {
            res.user = std::to_string(uid); 
        }
    }

    return res;
}