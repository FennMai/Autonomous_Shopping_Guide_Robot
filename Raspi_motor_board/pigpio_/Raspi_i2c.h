#ifndef Raspi_i2c_H
#define Raspi_i2c_H

#include <pigpio.h>
#include <cstdint>
#include <cstdio>

class Raspi_I2C {
public:
    Raspi_I2C() : fd_(-1), address_(0) {};

    // Ensure the I2C connection is closed when an instance is destroyed
    ~Raspi_I2C() {
        if (fd_ != -1) {
            i2cClose(fd_);
        }
    }

    // Initialize I2C communication
    bool init(uint8_t address) {
        fd_ = i2cOpen(1, address, 0); // 1 is the I2C bus number, adjust if necessary.
        if (fd_ < 0) {
            // Log error if unable to open I2C connection
            fprintf(stderr, "Unable to open I2C connection\n");
            return false;
        }
        address_ = address;
        return true;
    }

    // Write an 8-bit value to a register
    void WriteReg8(uint8_t reg, uint8_t value) {
        if (fd_ != -1) {
            i2cWriteByteData(fd_, reg, value);
        }
    }

    // Write a 16-bit value to a register
    void WriteReg16(uint8_t reg, uint16_t value) {
        if (fd_ != -1) {
            i2cWriteWordData(fd_, reg, value);
        }
    }

    // Write a single byte
    void WriteBit8(uint8_t value) {
        if (fd_ != -1) {
            i2cWriteByte(fd_, value);
        }
    }

    // Read an 8-bit value from a register
    uint8_t ReadReg8(uint8_t reg) {
        if (fd_ != -1) {
            return i2cReadByteData(fd_, reg);
        }
        return 0; // Consider how to handle this case; possibly throw an exception or define an error code.
    }

    // Read a 16-bit value from a register
    uint16_t ReadReg16(uint8_t reg) {
        if (fd_ != -1) {
            return i2cReadWordData(fd_, reg);
        }
        return 0; // Same here regarding error handling.
    }

    // Read a single byte
    uint8_t ReadBit8() {
        if (fd_ != -1) {
            return i2cReadByte(fd_);
        }
        return 0; // And here.
    }

    // Write a sequence of bytes from a buffer
    uint32_t Write(uint8_t* data, uint32_t size) {
        if (fd_ != -1) {
            return i2cWriteDevice(fd_, reinterpret_cast<char*>(data), size);
        }
        return 0; // Consider error handling here too.
    }

private:
    int fd_;
    uint8_t address_;
};

#endif // Raspi_i2c_H
