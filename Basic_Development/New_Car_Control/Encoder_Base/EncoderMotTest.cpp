/**
 * @author FENN MAI
 * @date 26/06/2024
 * @version Basic 4.6.2
 * @brief 左右电机控制测试 
 * pos-vol-pid control test
 */

#include <iostream>
#include <pigpio.h>
#include "EncoderShield.h"
#include "PID.h"

#ifndef M_PI
#define M_PI 3.1415927
#endif

// 轮子的直径（单位：米）
const double WHEEL_DIAMETER = 0.07;

// 编码器每圈脉冲数
const int PULSES_PER_REVOLUTION = 1560;

// 计算电机前进1米所需的脉冲数
int calculatePulsesForDistance(double distance) {
    double circumference = M_PI * WHEEL_DIAMETER;
    return static_cast<int>((distance / circumference) * PULSES_PER_REVOLUTION);
}

// 计算速度（假设每秒钟调用一次）
double calculateSpeed(int current_pulses, int previous_pulses, double delta_time) {
    return (current_pulses - previous_pulses) / delta_time;
}

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed" << std::endl;
        return 1;
    }

    EncoderShield shield;
    shield.begin();

    EncoderMotor *motor1 = shield.getEncoderMotor(1);
    EncoderMotor *motor2 = shield.getEncoderMotor(2);
    motor1->begin(1, 5, 6); // 设置电机1，编码器引脚为 GPIO 5 和 GPIO 6
    motor2->begin(2, 13, 19); // 设置电机2，编码器引脚为 GPIO 13 和 GPIO 19

    PID position_pid1(0.1, 0.01, 0.05); // 位置PID motor1
    PID speed_pid1(0.1, 0.01, 0.05);    // 速度PID motor1

    PID position_pid2(0.1, 0.01, 0.05); // 位置PID motor2
    PID speed_pid2(0.1, 0.01, 0.05);    // 速度PID motor2

    // 计算前进1米所需的脉冲数
    int target_pulses = calculatePulsesForDistance(1.0); 

    // 必须步骤，重置编码器位置，都设定为0
    motor1->resetEncoder();
    motor2->resetEncoder();

    gpioDelay(2500000);
    // 控制左轮（motor1）前进
    motor1->setSpeed(50);
    motor1->run(FORWARD);
    std::cout << "左轮-FORWARD" << std::endl;

    // 获取编码器的方向
    int dir1 = motor1->getDirection();
    std::cout << "左轮方向：" << dir1 << std::endl;
    
    gpioDelay(2500000);

    // 获取编码器的方向
    dir1 = motor1->getDirection();
    std::cout << "左轮方向：" << dir1 << std::endl;
    gpioDelay(2500000);
    // 停止电机1
    motor1->run(BRAKE);
    std::cout << "左轮-BRAKE" << std::endl;
    gpioDelay(5000000); 

    // 控制右轮（motor2）前进
    motor2->setSpeed(50);
    motor2->run(FORWARD);
    std::cout << "右轮-FORWARD" << std::endl;
    // 获取编码器的方向
    int dir2 = motor2->getDirection();
    std::cout << "右轮方向：" << dir2 << std::endl; 
    // 持续5s
    gpioDelay(2500000); 
    // 获取编码器的方向
    dir2 = motor2->getDirection();
    std::cout << "右轮方向：" << dir2 << std::endl; 
    gpioDelay(2500000);
    // 停止电机2
    motor2->run(BRAKE);
    std::cout << "右轮-RELEASE" << std::endl;

    // // 控制左轮（motor1）后退
    // motor1->setSpeed(50);
    // motor1->run(BACKWARD);
    // std::cout << "Left motor (motor1) running backward at speed 50" << std::endl;
    // gpioDelay(5000000); 
    // motor1->run(RELEASE);
    // std::cout << "Left motor (motor1) stopped" << std::endl;

    // // 控制右轮（motor2）后退
    // motor2->setSpeed(50);
    // motor2->run(BACKWARD);
    // std::cout << "Right motor (motor2) running backward at speed 50" << std::endl;
    // gpioDelay(5000000); 
    // motor2->run(RELEASE);
    // std::cout << "Right motor (motor2) stopped" << std::endl;

    // // 控制左右轮同时前进
    // motor1->setSpeed(50);
    // motor2->setSpeed(50);
    // motor1->run(FORWARD);
    // motor2->run(FORWARD);
    // std::cout << "Both motors running forward at speed 50" << std::endl;
    // gpioDelay(5000000); 
    // motor1->run(RELEASE);
    // motor2->run(RELEASE);
    // std::cout << "Both motors stopped" << std::endl;
    gpioTerminate();
    return 0;
}
