#include "MPU6050Filter.h"
#include <iostream>

MPU6050Filter::MPU6050Filter(int i2cBus, int address) {
    handle = i2cOpen(i2cBus, address, 0);
    if (handle < 0) {
        std::cerr << "I2C设备打开失败" << std::endl;
        throw std::runtime_error("Failed to open I2C device");
    }
    initialize();
}

MPU6050Filter::~MPU6050Filter() {
    i2cClose(handle);
    gpioTerminate();
}

void MPU6050Filter::initialize() {
    i2cWriteByteData(handle, 0x6B, 0x00); // 禁止睡眠模式
    reset();
}

void MPU6050Filter::reset() {
    angleX = angleY = angleZ = 0.0;
    lastUpdate = std::chrono::steady_clock::now();
}

void MPU6050Filter::updateAngles() {
    auto currentTime = std::chrono::steady_clock::now();
    double dt = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdate).count() / 1000.0;
    lastUpdate = currentTime;

    int gyro_x = read_word_2c(0x43);
    int gyro_y = read_word_2c(0x45);
    int gyro_z = read_word_2c(0x47);

    double gyro_x_scaled = gyro_x / 131.0;
    double gyro_y_scaled = gyro_y / 131.0;
    double gyro_z_scaled = gyro_z / 131.0;

    angleX += gyro_x_scaled * dt;
    angleY += gyro_y_scaled * dt;
    angleZ += gyro_z_scaled * dt;
}

void MPU6050Filter::printData() {
    std::cout << "角度: x=" << angleX << ", y=" << angleY << ", z=" << angleZ << std::endl;
}

int MPU6050Filter::read_word_2c(int addr) {
    int val = (i2cReadByteData(handle, addr) << 8) | i2cReadByteData(handle, addr + 1);
    if (val >= 0x8000) {
        val = -(65536 - val);
    }
    return val;
}

double MPU6050Filter::dist(double a, double b) {
    return sqrt(a * a + b * b);
}

double MPU6050Filter::get_y_rotation(double x, double y, double z) {
    double radians = atan2(x, dist(y, z));
    return -(radians * (180.0 / M_PI));
}

double MPU6050Filter::get_x_rotation(double x, double y, double z) {
    double radians = atan2(y, dist(x, z));
    return radians * (180.0 / M_PI);
}

double MPU6050Filter::getDeltaTime() {
    auto now = std::chrono::steady_clock::now();
    double dt = std::chrono::duration<double>(now - lastUpdate).count();
    lastUpdate = now;
    return dt;
}
double MPU6050Filter::getAngleX() const {
    return angleX;
}

double MPU6050Filter::getAngleY() const {
    return angleY;
}

double MPU6050Filter::getAngleZ() const {
    return angleZ;
}