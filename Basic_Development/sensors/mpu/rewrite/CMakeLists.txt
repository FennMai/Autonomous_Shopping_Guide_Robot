cmake_minimum_required(VERSION 3.18.4)
project(MPU6050_Async LANGUAGES CXX)  # Specify the project name and the language used

set(CMAKE_CXX_STANDARD 17)  # Use C++17 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags for different build types
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -no-pie")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pedantic -no-pie")

# Find PIGPIO library; assume it's installed and available on the system path
find_library(PIGPIO_LIBRARIES NAMES pigpio REQUIRED)
find_path(PIGPIO_INCLUDE_DIR NAMES pigpio.h REQUIRED)

# Include directories
include_directories(${PIGPIO_INCLUDE_DIR})

# Source files
set(SRC_FILES
    main.cpp
    MPU6050Filter.cpp
)

# Define the executable with source files
add_executable(MPU6050_Async ${SRC_FILES})

# Linking PIGPIO library
target_link_libraries(MPU6050_Async ${PIGPIO_LIBRARIES})
