cmake_minimum_required(VERSION 3.20)
project(temat_03 VERSION 1.0 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags - add warnings
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O2)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/inc)

# Create list of source files
set(SOURCES
        src/main.cpp
        src/task.cpp
        src/scheduler.cpp
)

# Create the executable
add_executable(scheduler ${SOURCES})

# Create a variable with the path to the data folder
set(DATA_DIR "${CMAKE_SOURCE_DIR}/data")

# Copy data files from data/ directory to the executable directory after building
add_custom_command(TARGET scheduler POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${DATA_DIR} $<TARGET_FILE_DIR:scheduler>/data)

# Display to the user where the data directory is
message(STATUS "Data will be copied to directory: $<TARGET_FILE_DIR:scheduler>/data")