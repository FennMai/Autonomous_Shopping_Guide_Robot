#ifndef DFRobot_DF2301Q_RPI_H
#define DFRobot_DF2301Q_RPI_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// 定义DFRobot_DF2301Q设备的I2C寄存器地址
#define DF2301Q_I2C_ADDR                 0x64  // I2C地址
#define DF2301Q_I2C_REG_CMDID            0x02  // 用于请求命令字ID的寄存器地址
#define DF2301Q_I2C_REG_PLAY_CMDID       0x03  // 通过命令字ID播放音频的寄存器地址
#define DF2301Q_I2C_REG_SET_MUTE         0x04  // 设置静音模式的寄存器
#define DF2301Q_I2C_REG_SET_VOLUME       0x05  // 设置音量的寄存器
#define DF2301Q_I2C_REG_WAKE_TIME        0x06  // 唤醒时间的寄存器地址
#define DF2301Q_I2C_REG_SLEEP_TIME       0x07  // 休眠时间的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_MODE    0x08  // 播放模式的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_STATUS  0x09  // 播放状态的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_TIMES   0x0A  // 播放次数的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_INDEX   0x0B  // 播放索引的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_VOLUME  0x0C  // 播放音量的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_EQ      0x0D  // 播放EQ的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_MODE2   0x0E  // 播放模式2的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_MODE3   0x0F  // 播放模式3的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_MODE4   0x10  // 播放模式4的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_MODE5   0x11  // 播放模式5的寄存器地址
#define DF2301Q_I2C_REG_PLAYBACK_MODE6   0x12  // 播放模式6的寄存器地址


class DFRobot_DF2301Q_RPi {
public:
    // 构造函数，初始化I2C设备
    DFRobot_DF2301Q_RPi(const char* i2cDevicePath, int i2cAddr = DF2301Q_I2C_ADDR);

    // 析构函数，关闭I2C设备
    ~DFRobot_DF2301Q_RPi();

    // 开始方法，可用于设备的初始化
    bool begin();

    // 获取命令字ID
    uint8_t getCMDID(void);

    // 根据命令字ID播放音频或执行命令
    void playByCMDID(uint8_t CMDID);

    // 根据需要添加其他成员函数...

private:
    int _file; // 保存打开的I2C设备文件的文件描述符
    int _i2cAddr; // I2C地址
};

#endif // DFRobot_DF2301Q_RPI_H