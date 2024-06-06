#include "PID.h"

PID::PID(double kp, double ki, double kd)
    : kp(kp), ki(ki), kd(kd), prev_error(0), integral(0) {
}

double PID::calculate(double setpoint, double pv) {
    double error = setpoint - pv;
    integral += error;
    double derivative = error - prev_error;
    prev_error = error;

    double output = kp * error + ki * integral + kd * derivative;
    return output;
}
