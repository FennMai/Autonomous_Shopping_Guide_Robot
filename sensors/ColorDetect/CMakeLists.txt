cmake_minimum_required(VERSION 3.1)
project(REDcolorMOV)  # 更新项目名称以反映当前的功能

set(CMAKE_CXX_STANDARD 11)  # C++11 是足够的，但也可以设置为更高版本如 C++14 或 C++17
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(OpenCV REQUIRED)  # 确保已安装OpenCV并且cmake能找到它
include_directories(${OpenCV_INCLUDE_DIRS})  # 包含OpenCV头文件目录

add_executable(REDcolorMOV Color_Mov.cpp)  # 更新可执行文件的名称

target_link_libraries(REDcolorMOV ${OpenCV_LIBS})  # 链接OpenCV库到你的项目
