/***
 * author : Basav
 * last modified date: 02/04/2024
 * 
 * Basic 3.0, date: 27/03/2024
 * changed the code from wiringPi to pigpio to control the servo and DC gear 
 * 
 */
#include "Raspi_i2c.h"

Raspi_I2C::Raspi_I2C() : fd_(-1), address_(0) {}

Raspi_I2C::~Raspi_I2C() {
    if (fd_ != -1) {
        i2cClose(fd_);
    }
}

bool Raspi_I2C::init(uint8_t address) {
    fd_ = i2cOpen(1, address, 0);
    if (fd_ < 0) {
        fprintf(stderr, "Unable to open I2C connection\n");
        return false;
    }
    address_ = address;
    return true;
}

void Raspi_I2C::WriteReg8(uint8_t reg, uint8_t value) {
    i2cWriteByteData(fd_, reg, value);
}

void Raspi_I2C::WriteReg16(uint8_t reg, uint16_t value) {
    i2cWriteWordData(fd_, reg, value);
}

void Raspi_I2C::WriteBit8(uint8_t value) {
    i2cWriteByte(fd_, value);
}

uint8_t Raspi_I2C::ReadReg8(uint8_t reg) {
    return i2cReadByteData(fd_, reg);
}

uint16_t Raspi_I2C::ReadReg16(uint8_t reg) {
    return i2cReadWordData(fd_, reg);
}

uint8_t Raspi_I2C::ReadBit8() {
    return i2cReadByte(fd_);
}

uint32_t Raspi_I2C::Write(uint8_t* data, uint32_t size) {
    return i2cWriteDevice(fd_, reinterpret_cast<char*>(data), size);
}
