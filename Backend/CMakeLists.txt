cmake_minimum_required(VERSION 3.14)

project(resource-monitor LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
        json 
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz 
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        )
FetchContent_MakeAvailable(json)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(resource-monitor
    "main.cpp"
    "Core/ResourceMonitor.cpp"
    "Components/CPUComponent.cpp"
    "Components/RAMComponent.cpp"
    "Components/ProcessComponent.cpp"
    )

target_link_libraries(resource-monitor PRIVATE 
    nlohmann_json::nlohmann_json
)