// DFRobot_DF2301Q.h
#ifndef DFRobot_DF2301Q_H
#define DFRobot_DF2301Q_H

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include <cstring>

#define DF2301Q_I2C_ADDR 0x64  // Define the I2C address for DF2301Q

class DFRobot_DF2301Q_I2C {
public:
    DFRobot_DF2301Q_I2C(const std::string& i2cBus = "/dev/i2c-1", uint8_t i2cAddr = DF2301Q_I2C_ADDR)
    : _i2cAddr(i2cAddr), _file(-1), _i2cBus(i2cBus) {}

    bool begin() {
        _file = open(_i2cBus.c_str(), O_RDWR);
        if (_file < 0) {
            perror("Failed to open the I2C bus");
            return false;
        }
        if (ioctl(_file, I2C_SLAVE, _i2cAddr) < 0) {
            perror("Failed to acquire bus access and/or talk to slave.");
            close(_file);
            _file = -1;
            return false;
        }
        return true;
    }

    void setVolume(uint8_t vol) {
        // Example command, adjust per your device's protocol
        uint8_t command[2] = {0x05, vol};  // Assuming register 0x05 controls volume
        write(_file, command, 2);
    }

    // Add additional methods following the pattern above

    ~DFRobot_DF2301Q_I2C() {
        if (_file != -1) {
            close(_file);
        }
    }

private:
    int _file;
    uint8_t _i2cAddr;
    std::string _i2cBus;
};

#endif // DFRobot_DF2301Q_H
