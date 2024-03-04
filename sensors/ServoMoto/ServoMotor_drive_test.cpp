#include <pigpio.h>
#include <unistd.h>
#include <iostream>

#define SERVO_PIN 18

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "Unable to initialize GPIO" << std::endl;
        return 1;
    }

    gpioSetMode(SERVO_PIN, PI_OUTPUT);

    // 将舵机转到90度角度
    int pulseWidth = 1500; // 对应90度的脉冲宽度
    gpioServo(SERVO_PIN, pulseWidth); 

    usleep(1000000); // 等待1秒钟

    gpioTerminate();

    return 0;
}

