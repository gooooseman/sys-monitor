cmake_minimum_required(VERSION 3.15)
project(SysMonitor CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(sys_monitor
    src/main.cpp
    src/MetricsCollector.cpp
    src/SnapshotManager.cpp
    src/Display.cpp
)

target_link_libraries(sys_monitor PRIVATE pthread)