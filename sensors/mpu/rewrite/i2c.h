/*
Author: Geo
Last Modified Date: 14/04/2024
Version 3.0, Date: 26/03/2024

Version 4.0, Date: 14/04/2024
I rewrite the I2Cdev class to make it more readable and easier to use.
*/
#ifndef I2CDEV_H_
#define I2CDEV_H_

#include <cstdint>
#include <iostream>
#include <fcntl.h>      // 提供文件控制功能，如 open() 函数
#include <unistd.h>     // 提供 UNIX 标准符号常量和类型，如 close() 函数
#include <sys/ioctl.h>  // 提供设备控制接口，如 ioctl() 函数
#include <linux/i2c-dev.h>  // Linux I2C 设备的定义
#include <errno.h>      // 错误号定义，用于错误处理
#include <cstring>      // 字符串操作函数，如 strerror()
#include <stdexcept>    // 标准异常定义

class I2Cdev {
public:
    // 构造函数，参数为设备文件路径，如 "/dev/i2c-1"
    explicit I2Cdev(const char* devicePath);

    // 析构函数，用于关闭已打开的设备文件
    ~I2Cdev();

    // 选择I2C设备，参数为设备地址
    bool selectDevice(uint8_t devAddr);

    // 从指定寄存器地址读取多个字节，参数分别为寄存器地址、数据缓冲区指针和数据长度
    bool readBytes(uint8_t regAddr, uint8_t* data, size_t length);

    // 向指定寄存器地址写入多个字节，参数分别为寄存器地址、数据缓冲区指针和数据长度
    bool writeBytes(uint8_t regAddr, const uint8_t* data, size_t length);

private:
    int fd; // 文件描述符，用于访问 I2C 设备

    // 内部使用的辅助函数，用于在读写操作中应用位掩码
    bool writeMaskedByte(uint8_t regAddr, uint8_t data, uint8_t mask);
    bool readMaskedByte(uint8_t regAddr, uint8_t* data, uint8_t mask);
};

I2Cdev::I2Cdev(const char* devicePath) {
    fd = open(devicePath, O_RDWR);  // 以读写方式打开设备文件
    if (fd < 0) {
        throw std::runtime_error("无法打开 I2C 总线：" + std::string(strerror(errno)));
    }
}

I2Cdev::~I2Cdev() {
    if (fd >= 0) {
        close(fd);  // 关闭文件描述符，释放资源
    }
}

bool I2Cdev::selectDevice(uint8_t devAddr) {
    if (ioctl(fd, I2C_SLAVE, devAddr) < 0) {
        std::cerr << "选择 I2C 设备失败：" << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool I2Cdev::readBytes(uint8_t regAddr, uint8_t* data, size_t length) {
    if (write(fd, &regAddr, 1) != 1) {
        std::cerr << "写入寄存器地址失败：" << strerror(errno) << std::endl;
        return false;
    }
    if (read(fd, data, length) != (ssize_t)length) {
        std::cerr << "从 I2C 总线读取失败：" << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool I2Cdev::writeBytes(uint8_t regAddr, const uint8_t* data, size_t length) {
    uint8_t* buffer = new uint8_t[length + 1];
    buffer[0] = regAddr;  // 将寄存器地址置于缓冲区首位
    memcpy(buffer + 1, data, length);  // 将数据复制到缓冲区
    if (write(fd, buffer, length + 1) != (ssize_t)(length + 1)) {
        std::cerr << "向 I2C 总线写入失败：" << strerror(errno) << std::endl;
        delete[] buffer;
        return false;
    }
    delete[] buffer;
    return true;
}

#endif // I2CDEV_H_
