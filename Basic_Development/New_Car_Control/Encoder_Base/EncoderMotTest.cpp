/** 
 * @author FENN MAI
 * @date 06/05/2024
 * @version Basic 4.5
 * @brief 编码电机初次测试
 */

#include <iostream>
#include <pigpio.h>
#include "EncoderShield.h"
#include "PID.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
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

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed" << std::endl;
        return 1;
    }

    EncoderShield shield;
    shield.begin();

    EncoderMotor *motor1 = shield.getEncoderMotor(1);
    motor1->begin(1, 5, 6); // 设置电机1，编码器引脚为 GPIO 5 和 GPIO 6

    PID pid(0.1, 0.01, 0.05);

    int target_pulses = calculatePulsesForDistance(1.0); // 计算前进1米所需的脉冲数
    motor1->resetEncoder();

    motor1->setSpeed(128);
    motor1->run(FORWARD);

    bool reached_target = false;
    while (!reached_target) {
        int actual_pulses = motor1->readEncoder();
        double error = target_pulses - actual_pulses;
        double speed = pid.calculate(target_pulses, actual_pulses);

        // 调整速度以更精确地接近目标
        if (abs(error) < 2000) { // 如果误差小于2000，减速
            speed = std::max(30.0, speed); // 保持一个最低速度以防止突然停止
        }

        motor1->setSpeed(static_cast<uint8_t>(std::min(std::max(speed, 0.0), 255.0)));

        int direction = motor1->getDirection();
        std::string direction_str = (direction == 1) ? "顺时针" : (direction == -1) ? "逆时针" : "静止";

        std::cout << "电机编号：1；理想前进值：" << target_pulses
                  << "；实际前进值：" << actual_pulses
                  << "；误差：" << error
                  << "；方向：" << direction_str << std::endl;

        if (abs(error) <50 && error < 0) { // 允许的误差范围
            reached_target = true;
        }

        gpioDelay(100 * 1000); // 延时100毫秒
    }

    motor1->run(RELEASE);
    gpioTerminate();
    return 0;
}
