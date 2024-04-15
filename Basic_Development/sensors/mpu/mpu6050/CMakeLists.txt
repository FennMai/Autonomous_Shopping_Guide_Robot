cmake_minimum_required(VERSION 3.1)  # 设置CMake的最小版本要求
project(MPU6050)  # 项目名称

set(CMAKE_CXX_STANDARD 17)  # 设置C++标准为C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制要求使用C++17标准

# 寻找pigpio库，确保其已安装
find_library(PIGPIO_LIBRARY NAMES pigpio REQUIRED)
if(NOT PIGPIO_LIBRARY)
    message(FATAL_ERROR "pigpio library not found")
endif()

# 寻找pthread库，用于多线程支持
find_package(Threads REQUIRED)

# 添加可执行文件
# 确保包括所有相关的源文件和头文件
add_executable(MPU6050 main.cpp KalmanFilter.h)  # 如果有KalmanFilter.cpp

# 包含目录，如果你的项目中有特定的头文件目录，请在这里添加
include_directories(
    ${CMAKE_SOURCE_DIR}/include  # 假设你的头文件放在项目目录的include文件夹下
)

# 链接库
target_link_libraries(MPU6050
    ${PIGPIO_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}  # 链接线程库
)

# 如果需要调试信息，可以添加编译器标志
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
