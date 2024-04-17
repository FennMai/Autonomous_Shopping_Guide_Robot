#include "arm_sys.h"
#include <iostream>
#include <unistd.h> // 用于 sleep 函数

int main() {
    // 假设 PCA9685 的四个电机通道分别为 0, 1, 2, 和 3
    mg90s arm_mtr1(0, 0.0); // 在通道 0 初始化电机1，起始角度为 0 度
    mg90s arm_mtr2(1, 0.0); // 在通道 1 初始化电机2，起始角度为 0 度
    mg90s arm_mtr3(2, 0.0); // 在通道 2 初始化电机3，起始角度为 0 度
    mg90s arm_mtr4(3, 0.0); // 在通道 3 初始化电机4，起始角度为 0 度
    
    // 回调函数
    auto onTargetReached = []() {
        std::cout << "目标角度已到达。" << std::endl;
    };

    // 异步设定每个电机的目标角度
    arm_mtr1.setTargetAngleAsync(45, onTargetReached); // 设定电机1到 45 度
    arm_mtr2.setTargetAngleAsync(-45, onTargetReached); // 设定电机2到 -45 度
    arm_mtr3.setTargetAngleAsync(30, onTargetReached); // 设定电机3到 30 度
    arm_mtr4.setTargetAngleAsync(-30, onTargetReached); // 设定电机4到 -30 度

    // 稍等片刻让电机达到目标角度
    sleep(5); // 睡眠 5 秒以等待运动完成


    return 0;
}
