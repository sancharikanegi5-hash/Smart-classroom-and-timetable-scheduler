cmake_minimum_required(VERSION 3.10)
project(SmartClassroomScheduler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Timetable
    ${CMAKE_CURRENT_SOURCE_DIR}/Faculty
    ${CMAKE_CURRENT_SOURCE_DIR}/Classroom
    ${CMAKE_CURRENT_SOURCE_DIR}/Subject
    ${CMAKE_CURRENT_SOURCE_DIR}/Student-Section
    ${CMAKE_CURRENT_SOURCE_DIR}/Scheduling
)

# Main executable
add_executable(scheduler
    main.cpp
    Timetable/timetable.cpp
    Backtracking/backtracking.cpp
)

