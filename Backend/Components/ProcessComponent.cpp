#include "ProcessComponent.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <cstring>

bool isNum(const char* str) {
    if(*str == '\0') return false;
    while(*str) {
        if (!std::isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

ProcessComponent::ProcessComponent(const char* path)  : Component(path) {}

std::vector<ProcessFileStat> ProcessComponent::ReadStat() {
    std::vector<ProcessFileStat> result{};
    
    struct stat st;
    struct dirent *dr;
    DIR* dir;
    dir = opendir(target_file_path);
    
    while((dr = readdir(dir)) != 0) {

        if (std::strcmp(dr->d_name, ".") == 0 || std::strcmp(dr->d_name, "..") == 0){
            continue;
        }

        std::string temp_path = std::string(target_file_path) + '/' + dr->d_name;
        if (lstat(temp_path.c_str(), &st) == -1) {
            continue;
        }

        if(S_ISDIR(st.st_mode)){
            try {
                int pid = std::stoi(dr->d_name);
                ProcessFileStat stat = ReadProcess(pid);
                
                if (stat.PID != 0) {
                    result.push_back(stat);
                }
            } catch(...) {
                continue;
            }

        }
    }
    closedir(dir);
    return result;
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
        std::istringstream iss(line);
        std::string temp_key;

        iss >> temp_key;

        if (temp_key == "Name:") {
            iss >> res.name;
            found_name = true;
        }
        if (temp_key == "State:") {
            iss >> res.state;
            found_state = true;
        }
        if (temp_key == "Uid:") {
            iss >> uid;
            found_uid = true;
        }

        if (found_name && found_state && found_uid) break;
    }

    if (found_uid) {
        struct passwd pw{};
        struct passwd* pw_result = nullptr;
        std::vector<char> buf(2048);
        
        if (getpwuid_r(uid, &pw, buf.data(), buf.size(), &pw_result) == 0 && pw_result) {
            res.user = pw_result->pw_name;
        } else {
            res.user = std::to_string(uid);
        }
    }

    return res;
}