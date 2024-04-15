#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <math.h>

#define MPU6050_ADDR 0x68
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B

// 读取16位的加速度计数据
int readAccelData(int handle, uint8_t reg) {
    int high = i2cReadByteData(handle, reg);
    int low = i2cReadByteData(handle, reg + 1);
    int value = (high << 8) + low;
    if (value >= 0x8000) value -= 0x10000; // 转换为有符号
    return value;
}

int main() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialization failed\n");
        return 1;
    }

    int handle = i2cOpen(1, MPU6050_ADDR, 0);
    if (handle < 0) {
        fprintf(stderr, "Failed to open I2C connection\n");
        gpioTerminate();
        return 1;
    }

    // Wake up MPU6050
    i2cWriteByteData(handle, PWR_MGMT_1, 0);

    while (1) {
        int accelX = readAccelData(handle, ACCEL_XOUT_H);
        int accelY = readAccelData(handle, ACCEL_XOUT_H + 2); // Y轴的高位地址
        int accelZ = readAccelData(handle, ACCEL_XOUT_H + 4); // Z轴的高位地址

        // 将加速度计数据转换为 g（9.8 m/s^2）
        double ax = accelX / 16384.0;
        double ay = accelY / 16384.0;
        double az = accelZ / 16384.0;

        // 计算翻滚角和俯仰角
        double roll = atan2(ay, az) * 57.2958;
        double pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 57.2958;

        printf("Accelerometer X: %f g, Y: %f g, Z: %f g\n", ax, ay, az);
        printf("Roll: %f degrees, Pitch: %f degrees\n", roll, pitch);

        sleep(1); // Delay for 1 second
    }

    i2cClose(handle);
    gpioTerminate();
    return 0;
}

