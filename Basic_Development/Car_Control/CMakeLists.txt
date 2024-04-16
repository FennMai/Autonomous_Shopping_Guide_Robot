cmake_minimum_required(VERSION 3.0)
project(tt)

set(CMAKE_CXX_STANDARD 11)

# Define the basic directories for output binaries and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Create corresponding directory structure
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib/x64)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/common)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/util)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tools)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/3rd_party)

# Specify source files
add_executable(tt main.cpp CarControl.cpp CarControl.hpp Emakefun_MotorDriver.cpp Emakefun_MotorDriver.h Emakefun_MotorShield.cpp Emakefun_MotorShield.h Raspi_i2c.cpp Raspi_i2c.h)

# Output target files to the bin directory
set_target_properties(tt PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# Find the pigpio library
find_library(PIGPIO_LIBRARY pigpio)

# If pigpio library is found, link it to the project
if(PIGPIO_LIBRARY)
    target_link_libraries(tt ${PIGPIO_LIBRARY})
else()
    message(FATAL_ERROR "pigpio library not found")
endif()

# Installation rules
install(TARGETS tt RUNTIME DESTINATION bin)
install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/ DESTINATION lib)
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION include)

