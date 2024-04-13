/***
 * author : Basav
 * last modified date: 11/04/2024
 * 
 * Basic 3.0, date: 10/04/2024
 * header for car control with distance as a parameter instead of time
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
    void moveForward(float distance_cm); // Updated for distance-based movement
    void turnLeft(int degrees); // Updated for degree-based turning
    void turnRight(int degrees); // Updated for degree-based turning
    void moveBackward(float distance_cm); // Updated for distance-based movement
    void stop(); // Stops the car immediately
    void setSpeed(int forwardBackwardSpeed, int turnSpeed); // Allows setting different speeds
    void cleanup(); // Cleanup resources

private:
    void setupServo();
    void setupDCMotors();
    void stopDCMotors();

    int _motorAPin1;
    int _motorAPin2;
    int _motorBPin1;
    int _motorBPin2;
    int _forwardBackwardSpeed; // Current speed for forward/backward movement
    int _turnSpeed; // Current speed for turning
    Emakefun_Servo* _servo;
    Emakefun_MotorShield _MS;
};

#endif // CAR_CONTROL_HPP
