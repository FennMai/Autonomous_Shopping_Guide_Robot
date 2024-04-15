/*
Author: Geo
Last Modified Date: 14/04/2024
Version 3.0, Date: 26/03/2024

Version 4.0, Date: 14/04/2024
I rewrite the I2Cdev class to make it more readable and easier to use.
*/
#include "I2Cdev.h"
#include <cstdlib> // 提供标准库，用于调用exit函数
#include <cerrno>  // 提供错误号

// 构造函数实现，尝试打开I2C设备文件，失败则抛出异常
I2Cdev::I2Cdev(const char* devicePath) : fd(open(devicePath, O_RDWR)) {
    if (fd < 0) {
        throw std::runtime_error("无法打开I2C设备：" + std::string(strerror(errno)));
    }
}

// 析构函数实现，负责关闭打开的设备文件描述符
I2Cdev::~I2Cdev() {
    if (fd >= 0) {
        close(fd);
    }
}

// 设置I2C从设备地址，用于通信
bool I2Cdev::selectDevice(uint8_t devAddr) {
    if (ioctl(fd, I2C_SLAVE, devAddr) < 0) {
        std::cerr << "设置I2C从设备地址失败：" << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

// 从I2C设备的指定寄存器读取多个字节
bool I2Cdev::readBytes(uint8_t regAddr, uint8_t* data, size_t length) {
    if (write(fd, &regAddr, 1) != 1) {  // 先写入寄存器地址
        std::cerr << "写入寄存器地址失败：" << strerror(errno) << std::endl;
        return false;
    }
    if (read(fd, data, length) != static_cast<int>(length)) {  // 读取数据
        std::cerr << "从I2C设备读取数据失败：" << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

// 向I2C设备的指定寄存器写入多个字节
bool I2Cdev::writeBytes(uint8_t regAddr, const uint8_t* data, size_t length) {
    uint8_t* buffer = new uint8_t[length + 1];  // 创建包含寄存器地址的缓冲区
    buffer[0] = regAddr;  // 设置寄存器地址
    memcpy(buffer + 1, data, length);  // 复制数据到缓冲区

    if (write(fd, buffer, length + 1) != static_cast<int>(length + 1)) {
        std::cerr << "向I2C设备写入数据失败：" << strerror(errno) << std::endl;
        delete[] buffer;  // 出错时释放内存
        return false;
    }

    delete[] buffer;  // 正常情况下释放内存
    return true;
}
