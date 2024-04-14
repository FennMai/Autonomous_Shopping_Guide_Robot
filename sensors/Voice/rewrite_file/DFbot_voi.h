/***
 * author : Geo 
 * last modified date: 14/04/2024
 * 
 * version 3.0, date: 26/03/2024
 * drive the DF2301Q
 * 
 * version 4.0, date: 14/04/2024
 */
#ifndef DFbot_voi_H
#define DFbot_voi_H

#include <iostream>
#include <vector>
#include <cstdint> // 用于 uint8_t 等类型
#include <pigpio.h> // 用于树莓派上的 I2C 通信

// 调试宏
#ifdef ENABLE_DBG
  #include <iostream>
  #define DBG(...) { std::cout << "[" << __FUNCTION__ << "(): " << __LINE__ << " ] " << __VA_ARGS__ << std::endl; }
#else
  #define DBG(...)
#endif

// 设备操作常量定义
#define DF2301Q_I2C_ADDR                     uint8_t(0x64)   //!< I2C 地址

#define DF2301Q_I2C_REG_CMDID                uint8_t(0x02)   //!< 请求命令字ID的寄存器地址
#define DF2301Q_I2C_REG_PLAY_CMDID           uint8_t(0x03)   //!< 通过命令字ID播放音频的寄存器地址
#define DF2301Q_I2C_REG_SET_MUTE             uint8_t(0x04)   //!< 设置静音模式的寄存器
#define DF2301Q_I2C_REG_SET_VOLUME           uint8_t(0x05)   //!< 设置音量的寄存器
#define DF2301Q_I2C_REG_WAKE_TIME            uint8_t(0x06)   //!< 设置唤醒时间的寄存器地址
#define DF2301Q_I2C_REG_SLEEP_TIME           uint8_t(0x07)   //!< 休眠时间的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_MODE        uint8_t(0x08)   //!< 播放模式的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_STATUS      uint8_t(0x09)   //!< 播放状态的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_TIMES       uint8_t(0x0A)   //!< 播放次数的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_INDEX       uint8_t(0x0B)   //!< 播放索引的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_VOLUME      uint8_t(0x0C)   //!< 播放音量的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_EQ          uint8_t(0x0D)   //!< 播放EQ的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_MODE2       uint8_t(0x0E)   //!< 播放模式2的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_MODE3       uint8_t(0x0F)   //!< 播放模式3的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_MODE4       uint8_t(0x10)   //!< 播放模式4的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_MODE5       uint8_t(0x11)   //!< 播放模式5的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_MODE6       uint8_t(0x12)   //!< 播放模式6的寄存器地址

class DFRobot_DF2301Q {
public:
  DFRobot_DF2301Q() {}
  virtual bool begin() = 0; // 在派生类中必须实现的初始化函数
};

class DFRobot_DF2301Q_RPi : public DFRobot_DF2301Q {
public:
  DFRobot_DF2301Q_RPi(uint8_t i2cBus, uint8_t deviceAddr);
  virtual ~DFRobot_DF2301Q_RPi();
  virtual bool begin() override;

  uint8_t getCMDID();
  void playByCMDID(uint8_t CMDID);
  uint8_t getWakeTime();
  void setWakeTime(uint8_t wakeTime);
  void setVolume(uint8_t volume);
  void setMuteMode(uint8_t mode);

protected:
  void writeReg(uint8_t reg, uint8_t value);
  bool readReg(uint8_t reg, uint8_t &value);

private:
  int _i2cHandle;       // I2C 设备句柄
  uint8_t _i2cBus;      // 树莓派上的 I2C 总线编号
  uint8_t _deviceAddr;  // I2C 设备地址
};

#endif // DFRobot_DF2301Q_H
