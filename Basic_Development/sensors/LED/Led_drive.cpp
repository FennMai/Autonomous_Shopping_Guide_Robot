/***
 * author : ZMai
 * drive the led in BCM 18 
*/
#include <iostream>
#include <pigpio.h>

#define PIN_LED 18

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed" << std::endl;
        return 1;
    }

    // 设置 GPIO 引脚为输出模式
    gpioSetMode(PIN_LED, PI_OUTPUT);

    // 检查 a 的值，如果为 1，则设置 GPIO 输出为高电平，LED 灯常亮
    int a = 1;
    if (a == 1) {
        gpioWrite(PIN_LED, 1);  // 将 GPIO 输出设置为高电平
    }

    // 等待用户按下回车键
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    // 关闭 pigpio 库
    gpioTerminate();
    return 0;
}
