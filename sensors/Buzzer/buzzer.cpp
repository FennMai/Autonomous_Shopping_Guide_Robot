#include <pigpio.h>
#include <iostream>

class Buzzer {
public:
    Buzzer(int pin) : pin_(pin) {
        if (gpioInitialise() < 0) {
            std::cerr << "Failed to initialize GPIO\n";
            exit(1);
        }
        gpioSetMode(pin_, PI_OUTPUT); // 设置蜂鸣器引脚为输出模式
        gpioWrite(pin_, 1); // 初始化为高电平（蜂鸣器不响）
    }

    ~Buzzer() {
        gpioTerminate(); // 结束时清理GPIO
    }

    void start() {
        gpioWrite(pin_, 0); // 低电平触发蜂鸣器
    }

    void stop() {
        gpioWrite(pin_, 1); // 高电平使蜂鸣器停止
    }

private:
    int pin_;
};

int main() {
    Buzzer buzzer(18); // 创建Buzzer对象，连接到GPIO18

    buzzer.start(); // 启动蜂鸣器
    time_sleep(1); // 延时1秒
    buzzer.stop(); // 停止蜂鸣器

    return 0;
}

