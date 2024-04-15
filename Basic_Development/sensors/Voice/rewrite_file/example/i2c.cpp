#include "DFRobot_DF2301Q.h"
#include <pigpio.h>
#include <iostream>
#include <unistd.h>  // 引入用于 sleep 函数的库，提供延时功能

// 定义I2C通信对象，指定I2C总线为1号，设备地址为0x64
DFRobot_DF2301Q_RPi DF2301Q(1, 0x64);  

int main() {
    // 初始化 pigpio 库，用于控制 GPIO
    if (gpioInitialise() < 0) {
        std::cerr << "Pigpio 初始化失败。\n";
        return 1;
    }

    std::cout << "串口通信已启动\n";

    // 初始化传感器，不断重试直到连接成功
    while (!DF2301Q.begin()) {
        std::cerr << "与设备通信失败，请检查连接\n";
        sleep(3);  // 连接失败时，等待3秒后重试
    }
    std::cout << "设备初始化成功！\n";

    // 设置声音音量
    DF2301Q.setVolume(4);  // 将音量设置为4

    // 设置静音模式
    DF2301Q.setMuteMode(0);  // 0为取消静音，1为静音

    // 设置唤醒时间
    DF2301Q.setWakeTime(15);  // 将唤醒时间设置为15单位

    // 获取并显示当前设置的唤醒时间
    uint8_t wakeTime = DF2301Q.getWakeTime();
    std::cout << "当前唤醒时间 = " << static_cast<int>(wakeTime) << "\n";

    // 根据命令ID播放音频
    DF2301Q.playByCMDID(23);  // 播放命令ID为23的音频

    // 主循环，周期性检查并显示命令ID
    while (true) {
        // 获取当前命令ID，如果非0则有有效命令
        uint8_t CMDID = DF2301Q.getCMDID();
        if (CMDID != 0) {
            std::cout << "当前命令ID = " << static_cast<int>(CMDID) << "\n";
        }
        sleep(3);  // 每3秒检查一次
    }

    // 程序结束前，正确地关闭 pigpio 库以释放资源
    gpioTerminate();  
    return 0;
}
