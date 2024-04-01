#include "Raspi_i2c.h"

// Remember to include <unistd.h> only if necessary for your platform.
#include <unistd.h>

void Raspi_I2C::init(uint8_t address) {
    address_ = address;
    fd_ = i2cOpen(1, address, 0); // Bus 1 is typically used on Raspberry Pi but adjust if necessary.
    if (fd_ < 0) {
        fprintf(stderr, "Error accessing 0x%02X: Check your I2C address\n", address);
    }
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
