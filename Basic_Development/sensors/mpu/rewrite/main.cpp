#include <iostream>
#include <pigpio.h>
#include "MPU6050Filter.h"
#include <future>
#include <functional>
#include <vector>

void updateAndProcess(MPU6050Filter& mpu6050, std::function<void(double, double, double)> callback) {
    mpu6050.updateAngles();
    callback(mpu6050.getAngleX(), mpu6050.getAngleY(), mpu6050.getAngleZ());
}

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "PIGPIO初始化失败" << std::endl;
        return 1;
    }

    try {
        MPU6050Filter mpu6050(1, 0x68);

        // 定义回调函数
        auto printCallback = [](double x, double y, double z) {
            std::cout << "角度: x=" << x << ", y=" << y << ", z=" << z << std::endl;
            if (std::abs(x) > 10) {
                std::cout << "注意：小车可能发生倾斜，x轴角度超过10度！" << std::endl;
            }
        };

        // 启动异步任务循环
        std::vector<std::future<void>> tasks;
        while (true) {
            tasks.push_back(std::async(std::launch::async, updateAndProcess, std::ref(mpu6050), printCallback));
            gpioDelay(400000); // 控制任务发起频率，避免过载
            // 清理已完成的任务
            tasks.erase(std::remove_if(tasks.begin(), tasks.end(), [](const std::future<void>& task) {
                return task.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
            }), tasks.end());
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "运行时错误: " << e.what() << std::endl;
        gpioTerminate();
        return 1;
    }

    gpioTerminate();
    return 0;
}

