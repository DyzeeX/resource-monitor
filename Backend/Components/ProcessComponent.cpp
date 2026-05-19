#include "ProcessComponent.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>

bool isNum(const char* str) {
    if(str == '\0') return false;
    while(*str) {
        if (!std::isdigit(*str)) return false;
        str++;
    }
}

ProcessComponent::ProcessComponent(const char* path)  : Component("/proc") {}

std::vector<ProcessFileStat> ProcessComponent::ReadStat() {

}

ProcessFileStat ProcessComponent::ReadProcess(int PID) {
    std::string status_path = std::string(target_file_path) + "/" + std::to_string(PID) + "/status";
    std::ifstream file(status_path);
    
    ProcessFileStat res{};
    if (!file.is_open()) return res;

    res.PID = PID;
    std::string line;
    uid_t uid = 0;
    bool found_name = false, found_state = false, found_uid = false;

    while(std::getline(file, line)) {
        std::istringstream file(line);
        std::string temp_key;

        file >> temp_key;

        if (temp_key == "Name:") {
            file >> res.name;
            found_name = true;
        }
        if (temp_key == "State:") {
            file >> res.state;
            found_state = true;
        }
        if (temp_key == "Uid:") {
            file >> uid;
            found_name = true;
        }

        if (found_name && found_state && found_uid) break;

        if(found_uid) {
            struct passwd* pw = getpwuid(uid);
            res.user = pw ? pw->pw_name : std::to_string(uid);
        }
    }

    return res;
}