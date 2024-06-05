/** 
 * 
 * @author FENN MAI
 * @date 06/05/2024
 * @version Basic 4.0
 * @brief 编码电机初次测试
 */

#ifndef _PID_H_
#define _PID_H_

class PID {
public:
    PID(double kp, double ki, double kd);
    double calculate(double setpoint, double measured_value);

private:
    double kp;
    double ki;
    double kd;
    double prev_error;
    double integral;
};

#endif
