cmake_minimum_required(VERSION 3.1)
project(ArmOpencv)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# 寻找OpenCV库
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)

# 添加头文件的搜索路径
include_directories(${OpenCV_INCLUDE_DIRS} ${LIBCAMERA_INCLUDE_DIRS})

# 添加libcamera库的路径
link_directories(${LIBCAMERA_LIBRARY_DIRS})

# 添加源代码文件到可执行文件
add_executable(ArmOpencv arm_opencv_cb.cpp libcam2opencv.cpp arm_sys.cpp)

# 查找pigpio库，确保其已安装
find_library(PIGPIO_LIBRARY NAMES pigpio REQUIRED)

# 如果找不到pigpio库，则停止构建并报错
if(NOT PIGPIO_LIBRARY)
    message(FATAL_ERROR "pigpio library not found")
endif()

# 链接OpenCV、libcamera和pigpio库
target_link_libraries(ArmOpencv ${OpenCV_LIBS} ${LIBCAMERA_LIBRARIES} ${PIGPIO_LIBRARY})

# 如果需要额外的编译选项或者定义，可以在这里添加
# add_definitions(-DUSE_SOME_FEATURE)
