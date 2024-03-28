/*!
 * author : Geo
 * last modified date: 28/03/2024 00:16
 * 
 * Basic 1.1, date: 27/03/2024
 * drive the DF2301Q
 * is a LED control program of DF2301Q.
 */
#include "DF23_voi.h" // 更新为正确的头文件名
#include <iostream>
#include <unistd.h> // 为了使用 usleep 函数

#define PIN_LED 18 // 树莓派上的GPIO引脚编号，用于LED

DFRobot_DF2301Q_RPi asr; // 使用默认I2C总线和设备地址

void setup() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed." << std::endl;
        return 1;
    }

    gpioSetMode(PIN_LED, PI_OUTPUT);
    gpioWrite(LED_PIN, 0); // 初始化LED为低电平

    // 初始化传感器
    if (!asr.begin()) {
        std::cout << "Communication with device failed, please check connection" << std::endl;
        gpioTerminate();
        exit(1);
    }
    std::cout << "Begin ok!" << std::endl;

    asr.setVolume(4);
    asr.setMuteMode(0);
    asr.setWakeTime(20);

    uint8_t wakeTime = asr.getWakeTime();
    std::cout << "wakeTime = " << static_cast<int>(wakeTime) << std::endl;
}

void loop() {
    uint8_t CMDID = asr.getCMDID();
    switch (CMDID) {
        case 117:
            gpioWrite(PIN_LED, 1); // 点亮LED
            std::cout << "received '打开灯光', command flag '117'" << std::endl;
            break;

        case 118:
            gpioWrite(PIN_LED, 0); // 熄灭LED
            std::cout << "received '关闭灯光', command flag '118'" << std::endl;
            break;

        default:
            if (CMDID != 0) {
                std::cout << "CMDID = " << static_cast<int>(CMDID) << std::endl;
            }
    }
    usleep(300000); // 等效于Arduino中的delay(300)，单位是微秒
}

int main() {
    setup();
    while (true) {
        loop();
    }
    gpioTerminate(); // 清理并释放资源
    return 0;
}
