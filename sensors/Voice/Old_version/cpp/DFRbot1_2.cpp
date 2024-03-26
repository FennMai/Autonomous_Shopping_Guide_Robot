#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "DFRobot_DF2301Q_RPi.h"

// 假设的DF2301Q模块I2C寄存器地址
#define DF2301Q_I2C_REG_VOLUME 0x01    // 音量寄存器
#define DF2301Q_I2C_REG_MUTE 0x02      // 静音模式寄存器
#define DF2301Q_I2C_REG_WAKE_TIME 0x03 // 唤醒时间寄存器

// 这里假设DFRobot_DF2301Q_RPi.h已经包含了必要的类定义
// 下面是对DFRobot_DF2301Q_RPi类的成员函数的实现

// 设置音量的函数
void DFRobot_DF2301Q_RPi::setVolume(uint8_t volume) {
    uint8_t data[2] = {DF2301Q_I2C_REG_VOLUME, volume};
    if (write(_file, data, 2) != 2) {
        std::cerr << "Failed to set volume on the I2C bus." << std::endl;
    }
}

// 设置静音模式的函数
void DFRobot_DF2301Q_RPi::setMuteMode(uint8_t mode) {
    uint8_t data[2] = {DF2301Q_I2C_REG_MUTE, mode};
    if (write(_file, data, 2) != 2) {
        std::cerr << "Failed to set mute mode on the I2C bus." << std::endl;
    }
}

// 设置唤醒时间的函数
void DFRobot_DF2301Q_RPi::setWakeTime(uint8_t time) {
    uint8_t data[2] = {DF2301Q_I2C_REG_WAKE_TIME, time};
    if (write(_file, data, 2) != 2) {
        std::cerr << "Failed to set wake time on the I2C bus." << std::endl;
    }
}

// main函数，模拟原Python脚本的运行逻辑
int main() {
    // 创建DFRobot_DF2301Q_RPi对象，这里仅假设设备地址为0x01，并且连接在/dev/i2c-1
    DFRobot_DF2301Q_RPi voice("/dev/i2c-1", 0x01);

    // 初始化语音模块
    if (!voice.begin()) {
        std::cerr << "Failed to initialize the voice module." << std::endl;
        return 1;
    }

    // 这里可以根据实际需要调用voice.playByCMDID()来播放指定ID的语音
    // voice.playByCMDID(0x01); // 演示，实际使用时请根据需要修改

    while (true) {
        uint8_t cmdID = voice.getCMDID();
        if (cmdID != 0) { // 如果有命令识别
            std::cout << "Current CMD ID: " << std::hex << (int)cmdID << std::endl;
            // 根据cmdID执行相应的动作，例如播放语音等
        }

        usleep(1000 * 1000);  // 1秒延迟，减缓循环速度
    }

    return 0;
}
