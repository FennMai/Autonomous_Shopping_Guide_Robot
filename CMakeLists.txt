cmake_minimum_required(VERSION 3.1.0)  
project(SSC LANGUAGES CXX)      

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包括子目录
add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(ui)


# 包括安装规则，方便安装和部署
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY assets/ DESTINATION assets)
install(DIRECTORY config/ DESTINATION config)

# 添加自定义目标，例如打包、清理等
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

# 
find_package (Threads)
