cmake_minimum_required(VERSION 3.10)
project(MPU6050Library VERSION 1.0 DESCRIPTION "MPU6050 I2C Library")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Define the library
add_library(MPU6050 SHARED
    MPU6050.cpp
    MPU6050.h
)

set_target_properties(MPU6050 PROPERTIES
    PUBLIC_HEADER MPU6050.h
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

target_include_directories(MPU6050 PRIVATE .)

# Link with the i2c library
target_link_libraries(MPU6050 PRIVATE i2c)

# Installation rules for the library and header files
install(TARGETS MPU6050
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# Example executable
add_executable(Example Example.cpp)
target_link_libraries(Example PRIVATE MPU6050 pthread)

# Installation rule for the executable
install(TARGETS Example RUNTIME DESTINATION bin)

# Uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

# Add clean files
set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
    "MPU6050.o"
    "libMPU6050.so"
    "Example"
    "Example.o"
)