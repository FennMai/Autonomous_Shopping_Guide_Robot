/***
 * author : Geo 
 * last modified date: 14/04/2024
 * 
 * version 3.0, date: 26/03/2024
 * 
 * version 4.0, date: 14/04/2024
 * Only the use of classes is defined
 */
#include "DFbot_voi.h"
#include <pigpio.h>
#include <unistd.h>  

// 构造函数，初始化I2C通信
DFRobot_DF2301Q_RPi::DFRobot_DF2301Q_RPi(uint8_t i2cBus, uint8_t deviceAddr) : _i2cBus(i2cBus), _deviceAddr(deviceAddr), _i2cHandle(-1) {}

// 析构函数，关闭I2C通信句柄
DFRobot_DF2301Q_RPi::~DFRobot_DF2301Q_RPi() {
    if (_i2cHandle >= 0) {
        i2cClose(_i2cHandle);  // 关闭I2C连接
    }
}

// 初始化函数，设置GPIO和I2C
bool DFRobot_DF2301Q_RPi::begin() {
    if (gpioInitialise() < 0) {
        return false;  // GPIO初始化失败返回false
    }
    _i2cHandle = i2cOpen(_i2cBus, _deviceAddr, 0);  // 打开I2C通道
    return _i2cHandle >= 0;  // 判断I2C句柄是否有效
}

// 获取当前命令ID
uint8_t DFRobot_DF2301Q_RPi::getCMDID() {
    uint8_t CMDID = 0;
    readReg(DF2301Q_I2C_REG_CMDID, CMDID);  // 从指定寄存器读取命令ID
    usleep(50000);  // 延时50毫秒，防止读取速率干扰声音模块其他功能
    return CMDID;
}

// 根据命令ID播放声音
void DFRobot_DF2301Q_RPi::playByCMDID(uint8_t CMDID) {
    writeReg(DF2301Q_I2C_REG_PLAY_CMDID, CMDID);  // 写入命令ID到寄存器
    sleep(1);  // 延时1秒
}

// 获取唤醒时间
uint8_t DFRobot_DF2301Q_RPi::getWakeTime() {
    uint8_t wakeTime = 0;
    readReg(DF2301Q_I2C_REG_WAKE_TIME, wakeTime);  // 从寄存器读取唤醒时间
    return wakeTime;
}

// 设置唤醒时间
void DFRobot_DF2301Q_RPi::setWakeTime(uint8_t wakeTime) {
    writeReg(DF2301Q_I2C_REG_WAKE_TIME, wakeTime);  // 将唤醒时间写入寄存器
    //Wake time value(1~255)
}

// 设置音量
void DFRobot_DF2301Q_RPi::setVolume(uint8_t volume) {
    writeReg(DF2301Q_I2C_REG_SET_VOLUME, volume);  // 将音量值写入寄存器 
    //Volume value(1~7)
}

// 设置静音模式
void DFRobot_DF2301Q_RPi::setMuteMode(uint8_t mode) {
    writeReg(DF2301Q_I2C_REG_SET_MUTE, mode);  // 将静音模式写入寄存器
    //0: unmute, 1: mute
}

// 通过I2C写寄存器
void DFRobot_DF2301Q_RPi::writeReg(uint8_t reg, uint8_t value) {
    i2cWriteByteData(_i2cHandle, reg, value);  // 使用pigpio库函数写数据到寄存器
}

// 通过I2C读寄存器
bool DFRobot_DF2301Q_RPi::readReg(uint8_t reg, uint8_t &value) {
    int result = i2cReadByteData(_i2cHandle, reg);  // 使用pigpio库函数从寄存器读数据
    if (result >= 0) {
        value = result;
        return true;  // 读取成功返回true
    }
    return false;  // 读取失败返回false
}
