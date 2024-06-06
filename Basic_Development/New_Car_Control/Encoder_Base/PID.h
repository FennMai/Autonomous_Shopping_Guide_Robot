#ifndef PID_H
#define PID_H

class PID {
public:
    PID(double kp, double ki, double kd);
    double calculate(double setpoint, double pv);

private:
    double kp;
    double ki;
    double kd;
    double prev_error;
    double integral;
};

#endif
