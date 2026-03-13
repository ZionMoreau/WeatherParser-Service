cmake_minimum_required(VERSION 3.24)
project(WeatherParser_Service)

set(CMAKE_CXX_STANDARD 17)

find_package(cppzmq CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(WeatherParser_Service
        WeatherParserService.cpp)

target_link_libraries(WeatherParser_Service PRIVATE
        cppzmq
        nlohmann_json::nlohmann_json
)
