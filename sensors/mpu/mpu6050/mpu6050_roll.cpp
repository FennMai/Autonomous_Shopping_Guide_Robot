/*
调整时间步长dt和卡尔曼滤波器的其他参数。
根据测试调整这两个参数。
Editor Geo
version 1.0
There are some claim function is wrong
version 1.1
fix it, but Acceleration data was wrong. I think it is because without initial.
version 1.2
add bad data detect and mpu initial
*/
#include <iostream>
#include <pigpio.h>
#include <cmath>
#include <unistd.h>
#include "KalmanFilter.h"

// MPU6050 I2C地址和寄存器定义
#define MPU6050_ADDR 0x68
#define PWR_MGMT_1 0x6B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H 0x43
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define MPU6050_GYRO_XOUT_H 0x43

// 卡尔曼滤波器对象
KalmanFilter kalmanX, kalmanY;

// 用于存储陀螺仪偏置的全局变量
float gyro_offsets[3] = {0.0, 0.0, 0.0};

void calculate_gyro_offsets(int handle) {
    int num_samples = 100;
    float sum_x = 0, sum_y = 0, sum_z = 0;
    char buffer[6];

    for (int i = 0; i < num_samples; ++i) {
        i2cReadI2CBlockData(handle, MPU6050_GYRO_XOUT_H, buffer, 6);
        int16_t gx = (buffer[0] << 8) | buffer[1];
        int16_t gy = (buffer[2] << 8) | buffer[3];
        int16_t gz = (buffer[4] << 8) | buffer[5];

        sum_x += gx;
        sum_y += gy;
        sum_z += gz;

        usleep(10000); // 每个样本间隔10毫秒
    }

    gyro_offsets[0] = sum_x / num_samples;
    gyro_offsets[1] = sum_y / num_samples;
    gyro_offsets[2] = sum_z / num_samples;
}

void mpu6050_reset(int handle) {
    i2cWriteByteData(handle, PWR_MGMT_1, 0x80);
    sleep(1); // 等待重置完成
}

void mpu6050_init(int handle) {
    mpu6050_reset(handle);
    i2cWriteByteData(handle, PWR_MGMT_1, 0x00); // 唤醒传感器
    i2cWriteByteData(handle, CONFIG, 0x01);     // 设置DLPF
    i2cWriteByteData(handle, GYRO_CONFIG, 0x00); // 设置陀螺仪灵敏度（±250度/秒）
    i2cWriteByteData(handle, ACCEL_CONFIG, 0x00); // 设置加速度计灵敏度（±2g）
    calculate_gyro_offsets(handle); // 校准陀螺仪
}

void read_mpu6050_data(int handle, double &roll, double &pitch, double &yaw, double accel[3]) {
    char buffer[14];
    if (i2cReadI2CBlockData(handle, ACCEL_XOUT_H, buffer, 14) != 14) {
        std::cerr << "Failed to read block data from MPU6050" << std::endl;
        return;
    }

    int16_t ax = (buffer[0] << 8) | buffer[1];
    int16_t ay = (buffer[2] << 8) | buffer[3];
    int16_t az = (buffer[4] << 8) | buffer[5];

    pitch = kalmanX.updateEstimate(atan2(ax, sqrt(ay * ay + az * az)) * 180 / M_PI);
    roll = kalmanY.updateEstimate(atan2(ay, sqrt(ax * ax + az * az)) * 180 / M_PI);
    yaw = atan2(sqrt(ax * ax + ay * ay), az) * 180 / M_PI;

    accel[0] = ax / 16384.0 * 9.80665;
    accel[1] = ay / 16384.0 * 9.80665;
    accel[2] = az / 16384.0 * 9.80665;
}

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "Failed to initialize GPIO" << std::endl;
        return 1;
    }

    int handle = i2cOpen(1, MPU6050_ADDR, 0);
    if (handle < 0) {
        std::cerr << "Failed to open I2C handle" << std::endl;
        gpioTerminate();
        return 1;
    }

    mpu6050_init(handle);

    double roll, pitch, yaw;
    double accel[3];

    while (true) {
        read_mpu6050_data(handle, roll, pitch, yaw, accel);
        std::cout << "Roll: " << roll << ", Pitch: " << pitch << ", Yaw: " << yaw << std::endl;
        std::cout << "Acceleration - X: " << accel[0] << ", Y: " << accel[1] << ", Z: " << accel[2] << std::endl;
        sleep(1);
    }

    i2cClose(handle);
    gpioTerminate();
    return 0;
}
