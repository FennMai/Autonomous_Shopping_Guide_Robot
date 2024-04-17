cmake_minimum_required(VERSION 3.10)
project(ColorDetection)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# 寻找OpenCV库
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# 寻找libcamera库
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)
include_directories(${LIBCAMERA_INCLUDE_DIRS})
link_directories(${LIBCAMERA_LIBRARY_DIRS})

# 寻找pigpio库
find_library(PIGPIO_LIBRARY NAMES pigpio REQUIRED)

# 指定项目的源文件
set(SOURCE_FILES
    HSC_colordet.cpp
    ColorDetectionCallback.cpp
    libcam2opencv.cpp
    arm_opencv_cb.cpp
    arm_sys.cpp
)

# 添加可执行文件
add_executable(ColorDetection ${SOURCE_FILES})

# 链接库到可执行文件
target_link_libraries(ColorDetection ${OpenCV_LIBS} ${LIBCAMERA_LIBRARIES} ${PIGPIO_LIBRARY})
