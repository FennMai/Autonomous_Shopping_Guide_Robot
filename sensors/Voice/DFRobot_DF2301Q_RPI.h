#ifndef DFRobot_DF2301Q_RPI_H
#define DFRobot_DF2301Q_RPI_H

#include <pigpio.h>
#include <cstdint>
#include <cstring> // For memcpy
#include <vector>

// Assuming these are the register addresses or other constants used
// for I2C communication with the device.
constexpr uint8_t DF2301Q_I2C_REG_CMDID = 0x01;
constexpr uint8_t DF2301Q_I2C_REG_PLAY_CMDID = 0x02;
constexpr uint8_t DF2301Q_I2C_REG_WAKE_TIME = 0x03;
constexpr uint8_t DF2301Q_I2C_REG_SET_VOLUME = 0x04;
constexpr uint8_t DF2301Q_I2C_REG_SET_MUTE = 0x05;

class DFRobot_DF2301Q_RPi {
public:
    // Initialize with the I2C bus and device address
    DFRobot_DF2301Q_RPi(uint8_t i2cBus = 1, uint8_t deviceAddr = 0x64); // Replace 0xXX with the actual I2C address

    // Destructor to clean up
    virtual ~DFRobot_DF2301Q_RPi();

    // Start communication
    bool begin();

    // Get command ID
    uint8_t getCMDID();

    // Play audio by command ID
    void playByCMDID(uint8_t CMDID);

    // Get wake time
    uint8_t getWakeTime();

    // Set wake time
    void setWakeTime(uint8_t wakeTime);

    // Set volume
    void setVolume(uint8_t volume);

    // Set mute mode
    void setMuteMode(uint8_t mode);

private:
    uint8_t _i2cBus;
    uint8_t _deviceAddr;
    int _i2cHandle;

    // Internal method to write a byte to a register
    void writeReg(uint8_t reg, uint8_t value);

    // Internal method to read a byte from a register
    uint8_t readReg(uint8_t reg);

    // Helper function to handle I2C start and end conditions in pigpio
    void i2cStart();
    void i2cStop();
};

#endif // DFRobot_DF2301Q_RPI_H
