#include <iostream>
#include <pigpio.h>
#include "MPU6050.h"

#define I2C_DEVICE 1  // 树莓派的I2C设备通常为1（/dev/i2c-1）
#define LED_PIN 19    // 根据你的接线选择合适的GPIO引脚编号

MPU6050 accelgyro(I2C_DEVICE, MPU6050_DEFAULT_ADDRESS);

void setup() {
    if (gpioInitialise() < 0) {
        std::cerr << "Failed to initialize pigpio." << std::endl;
        exit(1);
    }

    // 设置LED引脚为输出模式
    gpioSetMode(LED_PIN, PI_OUTPUT);

    // 初始化MPU6050设备
    accelgyro.initialize();
    std::cout << "Initializing I2C devices..." << std::endl;
    
    if (accelgyro.testConnection()) {
        std::cout << "MPU6050 connection successful" << std::endl;
    } else {
        std::cerr << "MPU6050 connection failed" << std::endl;
        gpioTerminate();  // 清理pigpio资源
        exit(1);
    }
}

void loop() {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    // 从MPU6050读取加速度和陀螺仪数据
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // 输出读取的数据
    std::cout << "Accel/Gyro: " << ax << " " << ay << " " << az << " " << gx << " " << gy << " " << gz << std::endl;

    // LED闪烁表示程序运行中
    static bool blinkState = false;
    blinkState = !blinkState;
    gpioWrite(LED_PIN, blinkState ? 1 : 0);

    // 简单的延迟
    gpioDelay(500000); // 延迟500毫秒
}

int main() {
    setup();
    while (true) {
        loop();
    }
    gpioTerminate(); // 在程序结束前清理资源
    return 0;
}