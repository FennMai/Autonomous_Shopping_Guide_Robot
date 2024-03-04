cmake_minimum_required(VERSION 3.0)
project(tt)

set(CMAKE_CXX_STANDARD 11)

# 定义输出的二进制文件(bin)和库文件(lib)的基本目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 创建对应的目录结构
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib/x64)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/common)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/util)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tools)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/3rd_party)

# 指定源文件
add_executable(tt mpu.cpp)

# 将目标文件输出到bin目录下
set_target_properties(tt PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# 查找pigpio库
find_library(PIGPIO_LIBRARY pigpio)

# 如果找到了pigpio库，则链接到项目
if(PIGPIO_LIBRARY)
    target_link_libraries(tt ${PIGPIO_LIBRARY})
else()
    message(FATAL_ERROR "pigpio library not found")
endif()

# 安装规则
install(TARGETS tt RUNTIME DESTINATION bin)
install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/ DESTINATION lib)
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION include)

