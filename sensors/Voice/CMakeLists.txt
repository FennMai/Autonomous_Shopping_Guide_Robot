cmake_minimum_required(VERSION 3.1)
project(tt)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找到 pigpio 库
find_library(PIGPIO_LIBRARY pigpio)

# 如果找不到 pigpio 库，打印错误信息
if(NOT PIGPIO_LIBRARY)
    message(FATAL_ERROR "pigpio library not found")
endif()

# 添加要编译的源文件
add_executable(tt i2c_Asy.cpp DFbot_voi.cpp)

# 链接 pigpio 库
target_link_libraries(tt ${PIGPIO_LIBRARY})

# 设置安装规则
install(TARGETS tt DESTINATION /usr/local/bin)
