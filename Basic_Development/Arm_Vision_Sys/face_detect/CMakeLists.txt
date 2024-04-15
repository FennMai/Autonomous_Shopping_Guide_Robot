cmake_minimum_required(VERSION 3.1)  
project(FaceDetection)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# 寻找OpenCV库
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)

# 添加头文件的搜索路径
include_directories(${OpenCV_INCLUDE_DIRS} ${LIBCAMERA_INCLUDE_DIRS})

# 可能需要手动添加libcamera库的路径
link_directories(${LIBCAMERA_LIBRARY_DIRS})

# 添加可执行文件
add_executable(FaceDetect test.cpp libcam2opencv.cpp)

# 链接OpenCV和libcamera库
target_link_libraries(FaceDetect ${OpenCV_LIBS} ${LIBCAMERA_LIBRARIES})
