cmake_minimum_required(VERSION 4.5)
project(tt)

set(CMAKE_CXX_STANDARD 11)

# Ensure the lib and include directories are created
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)


# Define the basic directories for output binaries and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


# Specify include directories
include_directories(${CMAKE_SOURCE_DIR})

# Specify source files
# tag-> for change
# Emakefun_MotorShield.cpp
add_executable(tt
    EncoderMotTest.cpp
    PID.cpp
    encoder.cpp
    EncoderShield.cpp
    Emakefun_MotorDriver.cpp
    Raspi_i2c.cpp)

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

