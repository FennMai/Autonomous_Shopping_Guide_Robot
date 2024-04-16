#include <iostream>
#include <cmath>
#include <pigpio.h>
#include "MPU6050Filter.h"
#include <future>
#include <functional>
#include <vector>

// 使用平滑滤波器
#define MOVING_AVERAGE_SIZE 5

double movingAverage(std::vector<double>& values, double newValue) {
    if (values.size() >= MOVING_AVERAGE_SIZE) {
        values.erase(values.begin());
    }
    values.push_back(newValue);
    double sum = 0.0;
    for (auto v : values) {
        sum += v;
    }
    return sum / values.size();
}

void updateAndProcess(MPU6050Filter& mpu6050, std::function<void(double, double, double)> callback) {
    static std::vector<double> xHist, yHist, zHist;
    mpu6050.updateAngles();
    double avgX = movingAverage(xHist, mpu6050.getAngleX());
    double avgY = movingAverage(yHist, mpu6050.getAngleY());
    double avgZ = movingAverage(zHist, mpu6050.getAngleZ());
    callback(avgX, avgY, avgZ);
}

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "PIGPIO初始化失败" << std::endl;
        return 1;
    }

    MPU6050Filter mpu6050(1, 0x68); // 创建MPU6050传感器的一个实例
    mpu6050.reset();  // 在开始测量前重置角度

    try {
        auto printCallback = [](double x, double y, double z) {
            const double angleThreshold = 5.0; // 角度变化阈值
            const double distanceFactor = 0.1; // 距离计算因子
            if (std::abs(x) > angleThreshold) {
                std::cout << "向左转了" << std::abs(x) << "度" << std::endl;
            } else {
                std::cout << "向前走了" << std::abs(z) * distanceFactor << "cm" << std::endl;
            }
        };

        std::vector<std::future<void>> tasks;
        while (true) {
            tasks.push_back(std::async(std::launch::async, updateAndProcess, std::ref(mpu6050), printCallback));
            gpioDelay(400000); // 控制任务发起频率，避免过载
            tasks.erase(std::remove_if(tasks.begin(), tasks.end(), [](const std::future<void>& task) {
                return task.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
            }), tasks.end());
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "运行时错误: " << e.what() << std::endl;
        gpioTerminate(); // 错误发生时清理GPIO资源
        return 1;
    }

    gpioTerminate(); // 程序正常结束也需要清理GPIO资源
    return 0;
}
