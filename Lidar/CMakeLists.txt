
# 设置CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(LidarProject LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)

# 设置可执行文件和库文件的输出目录
# 对于多配置环境（如Visual Studio），使用CMAKE_RUNTIME_OUTPUT_DIRECTORY_<Config>来指定
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib) # 对静态库有效

# 添加包含目录
include_directories(sdk/include sdk/src)
# include_directories(sdk/src/arch/linux)

# 查找sdk/src目录下的所有.cpp文件
file(GLOB_RECURSE SDK_SOURCES "sdk/src/*.cpp")

# 添加可执行文件
add_executable(mm test.cpp ${SDK_SOURCES})

# 如果有其他库文件需要链接，可以在这里添加
# target_link_libraries(LidarProject other_libraries)
# end
