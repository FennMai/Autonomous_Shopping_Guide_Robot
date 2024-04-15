/***
 * author : Basav
 * last modified date: 10/04/2024
 * 
 * Basic 3.0, date: 10/04/2024
 * header file for the CarControl 
 * 
 */
#ifndef CAR_CONTROL_HPP
#define CAR_CONTROL_HPP

#include "Emakefun_MotorShield.h"
#include <pigpio.h>

class CarControl {
public:
    CarControl();
    ~CarControl();
    void initialize();
    void moveForward(int duration);
    void turnLeft(int duration);
    void turnRight(int duration); 
    void moveBackward(int duration);
    void stop(); 
    void setSpeed(int speed);
    void cleanup();

private:
    void setupServo();
    void setupDCMotors();
    void stopDCMotors();

    int _motorAPin1;
    int _motorAPin2;
    int _motorBPin1;
    int _motorBPin2;
    int _currentSpeed; // to keep track of current speed
    Emakefun_Servo* _servo;
    Emakefun_MotorShield _MS;
};

#endif // CAR_CONTROL_HPP
