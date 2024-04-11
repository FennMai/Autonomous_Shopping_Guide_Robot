#!/bin/bash

# 清空
rm -rf build

# 创建构建目录
mkdir -p build

# 进入构建目录
cd build

# 运行CMake，将源目录指定为上级目录
cmake ..

# 构建项目
make

cd ../output
# ./mm --channel --serial /dev/ttyUSB0 460800
./mm

