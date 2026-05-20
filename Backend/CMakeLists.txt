cmake_minimum_required(VERSION 3.14)

project(resource-monitor LANGUAGES CXX)

include(FetchContent)

# JSON
FetchContent_Declare(
        json 
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz 
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        )
FetchContent_MakeAvailable(json)
# end JSON

# ASIO
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG        asio-1-30-2
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
)
FetchContent_MakeAvailable(asio)

add_library(asio_lib INTERFACE)
target_include_directories(asio_lib INTERFACE ${asio_SOURCE_DIR}/asio/include)
target_compile_definitions(asio_lib INTERFACE
    ASIO_STANDALONE
    ASIO_NO_DEPRECATED
)
set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include CACHE PATH "" FORCE)
# end ASIO

# check OpenSSL
find_package(OpenSSL)
if (NOT OPENSSL_FOUND)
    find_package(PkgConfig QUIET)
    if (PKG_CONFIG_FOUND)
        pkg_check_modules(OPENSSL openssl)
    endif()
endif()

if (OPENSSL_FOUND)
else()
    message(WARNING
        "OpenSSL not found. Install it with:\n"
        "  sudo apt install libssl-dev\n"
    )
endif()
# end check OpenSSL

# Crow
set(CROW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CROW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG        v1.3.0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
)
FetchContent_MakeAvailable(crow)

add_library(crow_lib INTERFACE)
target_include_directories(crow_lib INTERFACE ${crow_SOURCE_DIR}/include)
#end Crow

find_package(Threads REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(resource-monitor
    "main.cpp"
    "Core/ResourceMonitor.cpp"
    "Components/CPUComponent.cpp"
    "Components/RAMComponent.cpp"
    "Components/ProcessComponent.cpp"
    )

target_include_directories(resource-monitor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(resource-monitor PRIVATE 
    nlohmann_json::nlohmann_json
    asio_lib
    crow_lib
    Threads::Threads
    $<$<BOOL:${OPENSSL_FOUND}>:OpenSSL::SSL>
    $<$<BOOL:${OPENSSL_FOUND}>:OpenSSL::Crypto>
)

target_compile_definitions(resource-monitor PRIVATE
    CROW_USE_BOOST=0
    $<$<BOOL:${OPENSSL_FOUND}>:CROW_ENABLE_SSL>
)