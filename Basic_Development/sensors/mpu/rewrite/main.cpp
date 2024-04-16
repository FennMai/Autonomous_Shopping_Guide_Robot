#include <iostream>
#include <cmath>
#include <pigpio.h>
#include "MPU6050Filter.h"
#include <future>
#include <functional>
#include <vector>

// 函数用于异步更新MPU6050的角度，并通过回调函数输出
void updateAndProcess(MPU6050Filter& mpu6050, std::function<void(double, double, double)> callback) {
    mpu6050.updateAngles(); // 更新角度数据
    callback(mpu6050.getAngleX(), mpu6050.getAngleY(), mpu6050.getAngleZ()); // 调用回调函数
}

int main() {
    // 初始化PIGPIO库，如果失败则输出错误并退出
    if (gpioInitialise() < 0) {
        std::cerr << "PIGPIO初始化失败" << std::endl;
        return 1;
    }

    try {
        MPU6050Filter mpu6050(1, 0x68); // 创建MPU6050过滤器对象
        mpu6050.reset(); // 重置角度数据为0，以当前位置为坐标原点

        // 定义回调函数，用于处理角度数据
        auto printCallback = [](double x, double y, double z) {
            // 判断角度以决定输出内容
            if (std::abs(x) > 10) { // 假定x轴角度变化大于10度为有效转向
                std::cout << "向左转了" << std::abs(x) << "度" << std::endl;
            } else {
                std::cout << "向前走了" << std::abs(z) * 10 << "cm" << std::endl; // 假定z轴的变化与前进距离成比例
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
        gpioTerminate();  // 错误发生时清理GPIO资源
        return 1;
    }

    gpioTerminate();  // 程序正常结束也需要清理GPIO资源
    return 0;
}
