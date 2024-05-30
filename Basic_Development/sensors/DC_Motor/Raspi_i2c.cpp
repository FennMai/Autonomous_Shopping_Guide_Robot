#include "Raspi_i2c.h"  // 包含自定义的Raspi_I2C类头文件
#include <pigpio.h>     // 包含pigpio库头文件
#include <unistd.h>     // 包含Unix标准库头文件，提供sleep等函数

// Raspi_I2C类的构造函数，初始化成员变量并初始化pigpio库
Raspi_I2C::Raspi_I2C() : handle_(-1), address_(0) {
    gpioInitialise();  // 初始化pigpio库，如果初始化失败返回PIGPIO_INIT_FAILED
}

// Raspi_I2C类的析构函数，确保资源被正确释放
Raspi_I2C::~Raspi_I2C() {
    if (handle_ >= 0) {         // 如果I2C设备句柄有效
        i2cClose(handle_);      // 关闭I2C设备
    }
    gpioTerminate();            // 终止pigpio库，释放所有分配的资源
}

// 初始化I2C设备，设置I2C地址
void Raspi_I2C::init(uint8_t address) {
    address_ = address;                                 // 设置I2C设备地址
    handle_ = i2cOpen(1, address_, 0);                  // 打开I2C总线1上的设备，返回句柄
    if (handle_ < 0) {                                  // 如果打开失败
        printf("Error accessing 0x%02X: Check your I2C address \n", address);  // 打印错误信息
    }
}

// 向指定寄存器写入8位数据
void Raspi_I2C::WriteReg8(uint8_t reg, uint8_t value) {
    i2cWriteByteData(handle_, reg, value);              // 使用pigpio函数写入8位数据到寄存器
}

// 向指定寄存器写入16位数据
void Raspi_I2C::WriteReg16(uint8_t reg, uint16_t value) {
    i2cWriteWordData(handle_, reg, value);              // 使用pigpio函数写入16位数据到寄存器
}

// 写入单个8位数据
void Raspi_I2C::WriteBit8(uint8_t value) {
    i2cWriteByte(handle_, value);                       // 使用pigpio函数写入8位数据
}

// 从指定寄存器读取8位数据
uint8_t Raspi_I2C::ReadReg8(uint8_t reg) {
    return i2cReadByteData(handle_, reg);               // 使用pigpio函数读取8位数据从寄存器
}

// 从指定寄存器读取16位数据
uint16_t Raspi_I2C::ReadReg16(uint8_t reg) {
    return i2cReadWordData(handle_, reg);               // 使用pigpio函数读取16位数据从寄存器
}

// 读取单个8位数据
uint8_t Raspi_I2C::ReadBit8() {
    return i2cReadByte(handle_);                        // 使用pigpio函数读取8位数据
}

// 写入多个字节数据
uint32_t Raspi_I2C::Write(uint8_t* data, uint32_t size) {
    return i2cWriteDevice(handle_, reinterpret_cast<char*>(data), size);  // 使用pigpio函数写入多个字节数据到设备
}
