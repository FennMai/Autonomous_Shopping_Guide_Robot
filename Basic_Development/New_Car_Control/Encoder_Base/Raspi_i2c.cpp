/** 
 * 
 * @author FENN MAI
 * @date 06/05/2024
 * @version Basic 4.0
 * @brief 编码电机初次测试
 */


#include "Raspi_i2c.h"
#include <pigpio.h>
#include <iostream>

Raspi_I2C::Raspi_I2C() : fd_(-1), address_(0) {}

Raspi_I2C::~Raspi_I2C() {
    if (fd_ != -1) {
        i2cClose(fd_);
    }
}

// Raspi_I2C类的构造函数初始化
bool Raspi_I2C::init(uint8_t address) {
    // 使用pigpio库的i2cOpen函数打开I2C设备的连接
    // 第一个参数是I2C总线的编号（1表示树莓派上的I2C总线1）
    // 第二个参数是I2C设备的地址  
    // 第三个参数是标志位，此处为0表示没有特殊标志
    fd_ = i2cOpen(1, address, 0);
    
    // 检查i2cOpen函数的返回值，如果小于0则表示打开连接失败
    if (fd_ < 0) {
        // 使用标准错误流输出错误信息
        std::cerr << "Unable to open I2C connection to address " << static_cast<int>(address) << std::endl;
        // 返回false表示初始化失败
        return false;
    }
    
    // 如果连接成功，将设备地址存储在address_成员变量中
    address_ = address;
    // 返回true表示初始化成功
    return true;
}


void Raspi_I2C::WriteReg8(uint8_t reg, uint8_t value) {
    if (i2cWriteByteData(fd_, reg, value) != 0) {
        std::cerr << "Failed to write byte to register " << static_cast<int>(reg) << std::endl;
    }
}

void Raspi_I2C::WriteReg16(uint8_t reg, uint16_t value) {
    if (i2cWriteWordData(fd_, reg, value) != 0) {
        std::cerr << "Failed to write word to register " << static_cast<int>(reg) << std::endl;
    }
}

void Raspi_I2C::WriteBit8(uint8_t value) {
    if (i2cWriteByte(fd_, value) != 0) {
        std::cerr << "Failed to write byte " << static_cast<int>(value) << std::endl;
    }
}

uint8_t Raspi_I2C::ReadReg8(uint8_t reg) {
    int result = i2cReadByteData(fd_, reg);
    if (result < 0) {
        std::cerr << "Failed to read byte from register " << static_cast<int>(reg) << std::endl;
        return 0; // or appropriate error code
    }
    return static_cast<uint8_t>(result);
}

uint16_t Raspi_I2C::ReadReg16(uint8_t reg) {
    int result = i2cReadWordData(fd_, reg);
    if (result < 0) {
        std::cerr << "Failed to read word from register " << static_cast<int>(reg) << std::endl;
        return 0; // or appropriate error code
    }
    return static_cast<uint16_t>(result);
}

uint8_t Raspi_I2C::ReadBit8() {
    int result = i2cReadByte(fd_);
    if (result < 0) {
        std::cerr << "Failed to read byte" << std::endl;
        return 0; // or appropriate error code
    }
    return static_cast<uint8_t>(result);
}

uint32_t Raspi_I2C::Write(uint8_t* data, uint32_t size) {
    int result = i2cWriteDevice(fd_, reinterpret_cast<char*>(data), size);
    if (result < 0) {
        std::cerr << "Failed to write buffer" << std::endl;
        return 0; // or appropriate error code
    }
    return static_cast<uint32_t>(result);
}
