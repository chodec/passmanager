cmake_minimum_required(VERSION 3.14)
project(passmanager 
    VERSION 0.1.0 
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(passmanager src/main.cpp src/cli/parser.cpp src/core/app.cpp src/core/storage.cpp src/core/crypto.cpp)
target_include_directories(passmanager PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(passmanager nlohmann_json::nlohmann_json OpenSSL::SSL)
