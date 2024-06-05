/** 
 * 
 * @author FENN MAI
 * @date 06/05/2024
 * @version Basic 4.0
 * @brief 编码电机初次测试
 */


#include "PID.h"

// PID 构造函数
PID::PID(double kp, double ki, double kd) : kp(kp), ki(ki), kd(kd), prev_error(0), integral(0) {}

// 计算控制输出
double PID::calculate(double setpoint, double measured_value) {
    double error = setpoint - measured_value;
    integral += error;
    double derivative = error - prev_error;
    prev_error = error;
    return kp * error + ki * integral + kd * derivative;
}
