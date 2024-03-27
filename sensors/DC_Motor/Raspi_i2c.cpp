#include "Raspi_i2c.h"
#include <pigpio.h>
#include <unistd.h>

Raspi_I2C::Raspi_I2C() : handle_(-1), address_(0) {
    gpioInitialise();  // 初始化pigpio库
}

Raspi_I2C::~Raspi_I2C() {
    if (handle_ >= 0) {
        i2cClose(handle_);  // 关闭I2C设备
    }
    gpioTerminate();  // 终止pigpio库
}

void Raspi_I2C::init(uint8_t address) {
    address_ = address;
    handle_ = i2cOpen(1, address_, 0);  // 假设使用主I2C总线
    if (handle_ < 0) {
        printf("Error accessing 0x%02X: Check your I2C address \n", address);
    }
}

void Raspi_I2C::WriteReg8(uint8_t reg, uint8_t value) {
    i2cWriteByteData(handle_, reg, value);
}

void Raspi_I2C::WriteReg16(uint8_t reg, uint16_t value) {
    i2cWriteWordData(handle_, reg, value);
}

void Raspi_I2C::WriteBit8(uint8_t value) {
    i2cWriteByte(handle_, value);
}

uint8_t Raspi_I2C::ReadReg8(uint8_t reg) {
    return i2cReadByteData(handle_, reg);
}

uint16_t Raspi_I2C::ReadReg16(uint8_t reg) {
    return i2cReadWordData(handle_, reg);
}

uint8_t Raspi_I2C::ReadBit8() {
    return i2cReadByte(handle_);
}

uint32_t Raspi_I2C::Write(uint8_t* data, uint32_t size) {
    return i2cWriteDevice(handle_, reinterpret_cast<char*>(data), size);
}
