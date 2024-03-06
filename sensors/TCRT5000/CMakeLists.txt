cmake_minimum_required(VERSION 3.0)
project(tt)

set(CMAKE_CXX_STANDARD 11)

# Ensure the output directory for the executable exists
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Specify the source files
add_executable(tt main.cpp TCRT5000.cpp)

# Set the output directory for the executable
set_target_properties(tt PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Find the pigpio library
find_library(PIGPIO_LIBRARY pigpio)

