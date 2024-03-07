#include <pigpio.h>
#include <iostream>
#include <unistd.h> // 对于Unix/Linux系统

class Buzzer {
public:
    Buzzer(int pin) : pin_(pin) {
        if (gpioInitialise() < 0) {
            std::cerr << "Failed to initialize GPIO\n";
            exit(1);
        }
        gpioSetMode(pin_, PI_OUTPUT);
        gpioWrite(pin_, 1); // 确保蜂鸣器初始状态为不发声
    }

    ~Buzzer() {
        gpioWrite(pin_, 1); // 确保蜂鸣器关闭
        gpioTerminate(); // 清理GPIO资源
    }

    void dot() {
        gpioWrite(pin_, 0); // 开始发声
        usleep(200000); // 持续200毫秒
        gpioWrite(pin_, 1); // 停止发声
        usleep(200000); // 字符间的延时
    }

    void dash() {
        gpioWrite(pin_, 0); // 开始发声
        usleep(600000); // 持续600毫秒
        gpioWrite(pin_, 1); // 停止发声
        usleep(200000); // 字符间的延时
    }

    void playMorseCode(const std::string& code) {
        for (char c : code) {
            if (c == '.') {
                dot();
            } else if (c == '-') {
                dash();
            }
        }
        usleep(600000); // 字母间的额外延时
    }

private:
    int pin_;
};

int main() {
    Buzzer buzzer(25); // 假设蜂鸣器连接到GPIO25引脚

    // "DOCTOR" 对应的莫斯电码
    std::string morseCode[] = {"-..", "---", "-.-.", "-", "---", ".-."};

    for (const std::string& code : morseCode) {
        buzzer.playMorseCode(code);
        usleep(1400000); // 单词间的额外延时
    }

    return 0;
}

