/***
 * Author : Geo 
 * last modify: 14/04/2024
 * 
 * Version 3.0, date: 26/03/2024
 * drive DF2301Q device
 * multithreading
 */
#include "DFbot_voi.h" // 自定义的设备控制头文件
#include <iostream>  // 包含标准输入输出流
#include <pigpio.h>  // 树莓派GPIO控制库
#include <thread>    // C++11线程库
#include <chrono>    // 包含chrono库，用于处理时间

// 函数定义：定期检查设备发出的命令ID
void checkCMDID(DFRobot_DF2301Q_RPi& device) {
    while (true) {
        uint8_t CMDID = device.getCMDID(); // 获取当前命令ID
        if (CMDID != 0) {
            std::cout << "CMDID = " << static_cast<int>(CMDID) << std::endl; // 如果命令ID不为0，则输出
        }
        std::this_thread::sleep_for(std::chrono::seconds(3)); // 每3秒检查一次
    }
}

// 主函数
int main() {
    if (gpioInitialise() < 0) { // 初始化GPIO
        std::cerr << "pigpio 初始化失败。" << std::endl;
        return 1; // 初始化失败，返回1
    }

    // 创建设备控制对象，指定I2C总线1和设备地址0x64
    DFRobot_DF2301Q_RPi DF2301Q(1, 0x64);

    // 尝试开始与设备的通信
    if (!DF2301Q.begin()) {
        std::cerr << "与设备通信失败，请检查连接" << std::endl;
        gpioTerminate(); // 清理GPIO资源
        return 1; // 通信失败，返回1
    }
    std::cout << "设备初始化成功！" << std::endl;

    // 设置设备音量、静音模式和唤醒时间
    DF2301Q.setVolume(3);
    DF2301Q.setMuteMode(0);
    DF2301Q.setWakeTime(15);

    // 获取并显示设备的唤醒时间
    uint8_t wakeTime = DF2301Q.getWakeTime();
    std::cout << "当前唤醒时间 = " << static_cast<int>(wakeTime) << std::endl;
    
    if (CMDID == 5) {
    // 条件1满足时的代码
    DF2301Q.playByCMDID(9);
    cout << "mapping start" << endl;
} else if (CMDID == 6) {
    // 条件2满足时的代码
    DF2301Q.playByCMDID(10);
    cout << "moving to banana" << endl;
} else if (CMDID == 7) {
    // 条件3满足时的代码
    DF2301Q.playByCMDID(11);
    cout << "执行条件3对应的操作" << endl;
} else if (CMDID == 8) {
    // 条件4满足时的代码
    DF2301Q.playByCMDID(12);
    cout << "执行条件4对应的操作" << endl;
} else {
    // 所有条件都不满足时的代码
    cout << "没有条件被满足" << endl;
}

    // 根据命令ID播放音频（示例中为ID 23）
    //DF2301Q.playByCMDID(23);

    // 创建一个新线程，用于周期性地检查命令ID
    std::thread checkCMDIDThread(checkCMDID, std::ref(DF2301Q));

    // 主线程继续执行，这里仅演示等待子线程结束（实际上子线程是无限循环的）
    checkCMDIDThread.join(); // 在实际应用中，应该提供一种优雅的退出机制

    gpioTerminate(); // 程序结束前清理GPIO资源
    return 0;
}
