/***
 * author : Geo ZMai
 * last modified date: 26/03/2024
 * 
 * Basic 3.0, date: 26/03/2024
 * drive the DF2301Q
 * 
 */
#ifndef DFRobot_DF2301Q_RPI_H
#define DFRobot_DF2301Q_RPI_H

#include <pigpio.h>
#include <cstdint>

// 定义寄存器地址和其他必要的常量
constexpr uint8_t DF2301Q_I2C_ADDR = 0x64; // I2C地址

constexpr uint8_t DF2301Q_I2C_REG_CMDID = 0x02;
constexpr uint8_t DF2301Q_I2C_REG_PLAY_CMDID = 0x03;
constexpr uint8_t DF2301Q_I2C_REG_SET_MUTE = 0x04;
constexpr uint8_t DF2301Q_I2C_REG_SET_VOLUME = 0x05;
constexpr uint8_t DF2301Q_I2C_REG_WAKE_TIME = 0x06;

class DFRobot_DF2301Q_RPi {
public:
    DFRobot_DF2301Q_RPi(uint8_t i2cBus = 1, uint8_t deviceAddr = DF2301Q_I2C_ADDR);
    virtual ~DFRobot_DF2301Q_RPi();

    // 初始化函数
    bool begin();

    // 获取命令词ID
    uint8_t getCMDID();

    // 根据命令词ID播放对应回复音频
    void playByCMDID(uint8_t CMDID);

    // 获取唤醒时长
    uint8_t getWakeTime();

    // 设置唤醒时长
    void setWakeTime(uint8_t wakeTime);

    // 设置音量
    void setVolume(uint8_t volume);

    // 设置静音模式
    void setMuteMode(uint8_t mode);

protected:
    // 写寄存器函数
    void writeReg(uint8_t reg, uint8_t value);

    // 读寄存器函数
    // uint8_t readReg(uint8_t reg);
// 在 DFRobot_DF2301Q_RPi.h 中调整
    bool readReg(uint8_t reg, uint8_t &data);

private:
    uint8_t _i2cBus;
    uint8_t _deviceAddr;
    int _i2cHandle; // pigpio库的I2C句柄
};

#endif // DFRobot_DF2301Q_RPI_H
