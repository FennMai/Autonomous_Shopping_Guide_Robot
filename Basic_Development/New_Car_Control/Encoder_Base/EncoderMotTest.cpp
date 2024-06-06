/** 
 * @author FENN MAI
 * @date 06/05/2024
 * @version Basic 4.6
 * @brief pos-vol-pid control test
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

    int target_pulses = calculatePulsesForDistance(1.0); // 计算前进1米所需的脉冲数
    motor1->resetEncoder();
    motor2->resetEncoder();

    int previous_pulses1 = 0;
    int previous_pulses2 = 0;
    double delta_time = 0.1; // 假设每0.1秒进行一次速度计算

    motor1->setSpeed(128);
    motor2->setSpeed(128);
    motor1->run(FORWARD);
    motor2->run(FORWARD);

    bool reached_target1 = false;
    bool reached_target2 = false;
    while (!reached_target1 || !reached_target2) {
        // Motor 1
        int actual_pulses1 = motor1->readEncoder();
        double position_error1 = target_pulses - actual_pulses1;
        double desired_speed1 = position_pid1.calculate(target_pulses, actual_pulses1);
        double actual_speed1 = calculateSpeed(actual_pulses1, previous_pulses1, delta_time);
        previous_pulses1 = actual_pulses1;
        double pwm_value1 = speed_pid1.calculate(desired_speed1, actual_speed1);
        motor1->setSpeed(static_cast<uint8_t>(std::min(std::max(pwm_value1, 0.0), 255.0)));

        int direction1 = motor1->getDirection();
        std::string direction_str1 = (direction1 == 1) ? "顺时针" : (direction1 == -1) ? "逆时针" : "静止";

        std::cout << "电机编号：1；理想前进值：" << target_pulses
                  << "；实际前进值：" << actual_pulses1
                  << "；误差：" << position_error1
                  << "；方向：" << direction_str1
                  << "；PWM值：" << pwm_value1 << std::endl;

        // Motor 2
        int actual_pulses2 = motor2->readEncoder();
        double position_error2 = target_pulses - actual_pulses2;
        double desired_speed2 = position_pid2.calculate(target_pulses, actual_pulses2);
        double actual_speed2 = calculateSpeed(actual_pulses2, previous_pulses2, delta_time);
        previous_pulses2 = actual_pulses2;
        double pwm_value2 = speed_pid2.calculate(desired_speed2, actual_speed2);
        motor2->setSpeed(static_cast<uint8_t>(std::min(std::max(pwm_value2, 0.0), 255.0)));

        int direction2 = motor2->getDirection();
        std::string direction_str2 = (direction2 == 1) ? "顺时针" : (direction2 == -1) ? "逆时针" : "静止";

        std::cout << "电机编号：2；理想前进值：" << target_pulses
                  << "；实际前进值：" << actual_pulses2
                  << "；误差：" << position_error2
                  << "；方向：" << direction_str2
                  << "；PWM值：" << pwm_value2 << std::endl;

        // 判断是否达到目标位置
        if (abs(position_error1) < 618) { // 允许的误差范围
            reached_target1 = true;
        }
        if (abs(position_error2) < 618) { // 允许的误差范围
            reached_target2 = true;
        }

        gpioDelay(static_cast<unsigned int>(delta_time * 1000 * 1000)); // 延时delta_time秒
    }

    motor1->run(RELEASE);
    motor2->run(RELEASE);
    gpioTerminate();
    return 0;
}
