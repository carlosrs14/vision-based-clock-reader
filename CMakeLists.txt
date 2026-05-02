cmake_minimum_required(VERSION 3.10)
project(ClockVision)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/clock_algorithm.cpp
)

add_executable(app ${SOURCES})

target_link_libraries(
    app
    ${OpenCV_LIBS}
)