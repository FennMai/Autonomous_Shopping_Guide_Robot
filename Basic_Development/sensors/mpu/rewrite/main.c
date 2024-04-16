#include <iostream>
#include <cmath>
#include <pigpio.h>

// 定义MPU6050的I2C地址
constexpr int MPU6050_ADDRESS = 0x68;

// 读取两个字节并合成一个整数
int read_word_2c(int handle, int addr) {
    int val = (i2cReadByteData(handle, addr) << 8) | i2cReadByteData(handle, addr + 1);
    if (val >= 0x8000) {
        val = -(65536 - val);
    }
    return val;
}

// 计算两个数的距离
double dist(double a, double b) {
    return sqrt(a * a + b * b);
}

// 获取Y轴旋转角度
double get_y_rotation(double x, double y, double z) {
    double radians = atan2(x, dist(y, z));
    return -(radians * (180.0 / M_PI));
}

// 获取X轴旋转角度
double get_x_rotation(double x, double y, double z) {
    double radians = atan2(y, dist(x, z));
    return radians * (180.0 / M_PI);
}

int main() {
    // 初始化PIGPIO库
    if (gpioInitialise() < 0) {
        std::cerr << "PIGPIO初始化失败" << std::endl;
        return 1;
    }

    // 设置并打开I2C设备
    int handle = i2cOpen(1, MPU6050_ADDRESS, 0);
    if (handle < 0) {
        std::cerr << "I2C设备打开失败" << std::endl;
        gpioTerminate();
        return 1;
    }

    // 禁止睡眠模式
    i2cWriteByteData(handle, 0x6B, 0x00);

    while (true) {
        int accl_x = read_word_2c(handle, 0x3B);
        int accl_y = read_word_2c(handle, 0x3D);
        int accl_z = read_word_2c(handle, 0x3F);

        double accl_x_scaled = accl_x / 16384.0;
        double accl_y_scaled = accl_y / 16384.0;
        double accl_z_scaled = accl_z / 16384.0;

        std::cout << "加速度：x=" << accl_x_scaled << ", y=" << accl_y_scaled << ", z=" << accl_z_scaled << std::endl;
        std::cout << "旋转度数: x=" << get_x_rotation(accl_x_scaled, accl_y_scaled, accl_z_scaled) 
                  << ", y=" << get_y_rotation(accl_x_scaled, accl_y_scaled, accl_z_scaled) << std::endl;

        int gyro_x = read_word_2c(handle, 0x43);
        int gyro_y = read_word_2c(handle, 0x45);
        int gyro_z = read_word_2c(handle, 0x47);

        double gyro_x_scaled = gyro_x / 131.0;
        double gyro_y_scaled = gyro_y / 131.0;
        double gyro_z_scaled = gyro_z / 131.0;

        std::cout << "角速度: x=" << gyro_x_scaled << ", y=" << gyro_y_scaled << ", z=" << gyro_z_scaled << std::endl;

        int temp = read_word_2c(handle, 0x41);
        double temp_c = 36.53 + temp / 340.0;
        std::cout << "温度: " << temp_c << "\n\n";

        gpioDelay(400000); // 延时400ms
    }

    i2cClose(handle); // 关闭I2C设备
    gpioTerminate(); // 结束PIGPIO使用
    return 0;
}
