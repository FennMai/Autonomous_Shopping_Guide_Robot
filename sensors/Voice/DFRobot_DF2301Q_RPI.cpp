#include "DFRobot_DF2301Q_RPI.h"
#include <pigpio.h>
#include <iostream>
#include <unistd.h> // For sleep()

DFRobot_DF2301Q_RPi::DFRobot_DF2301Q_RPi(uint8_t i2cBus, uint8_t deviceAddr)
    : _i2cBus(i2cBus), _deviceAddr(deviceAddr), _i2cHandle(-1) {}

DFRobot_DF2301Q_RPi::~DFRobot_DF2301Q_RPi() {
    if (_i2cHandle >= 0) {
        i2cClose(_i2cHandle);
    }
}

bool DFRobot_DF2301Q_RPi::begin() {
    if (gpioInitialise() < 0) {
        return false;
    }
    _i2cHandle = i2cOpen(_i2cBus, _deviceAddr, 0);
    return _i2cHandle >= 0;
}

uint8_t DFRobot_DF2301Q_RPi::getCMDID() {
    uint8_t cmdId = 0;
    readReg(DF2301Q_I2C_REG_CMDID, cmdId);
    gpioDelay(50000); // 50 ms delay
    return cmdId;
}

void DFRobot_DF2301Q_RPi::playByCMDID(uint8_t CMDID) {
    writeReg(DF2301Q_I2C_REG_PLAY_CMDID, CMDID);
    sleep(1); // 1 second delay
}

uint8_t DFRobot_DF2301Q_RPi::getWakeTime() {
    uint8_t wakeTime = 0;
    readReg(DF2301Q_I2C_REG_WAKE_TIME, wakeTime);
    return wakeTime;
}

void DFRobot_DF2301Q_RPi::setWakeTime(uint8_t wakeTime) {
    writeReg(DF2301Q_I2C_REG_WAKE_TIME, wakeTime);
}

void DFRobot_DF2301Q_RPi::setVolume(uint8_t volume) {
    writeReg(DF2301Q_I2C_REG_SET_VOLUME, volume);
}

void DFRobot_DF2301Q_RPi::setMuteMode(uint8_t mode) {
    writeReg(DF2301Q_I2C_REG_SET_MUTE, mode);
}

void DFRobot_DF2301Q_RPi::writeReg(uint8_t reg, uint8_t value) {
    if (_i2cHandle >= 0) {
        i2cWriteByteData(_i2cHandle, reg, value);
    }
}

uint8_t DFRobot_DF2301Q_RPi::readReg(uint8_t reg) {
    if (_i2cHandle >= 0) {
        return i2cReadByteData(_i2cHandle, reg);
    }
    return 0;
}
