/***
 * author : Basav
 * last modified date: 12/04/2024
 * 
 * Basic 3.0, date: 10/04/2024
 * header for car control with async and callback
 * 
 */
#ifndef CAR_CONTROL_HPP
#define CAR_CONTROL_HPP

#include "Emakefun_MotorShield.h"
#include <pigpio.h>
#include <functional>

class CarControl {
public:
    CarControl();
    ~CarControl();
    
    void initialize();
    void moveForward(float distance_cm, std::function<void()> callback);
    void moveBackward(float distance_cm, std::function<void()> callback);
    void turnRight(int degrees, std::function<void()> callback);
    void turnLeft(int degrees, std::function<void()> callback);
    void stop();
    void setSpeed(int forwardBackwardSpeed, int turnSpeed);
    void cleanup();

private:
    void setupServo();  // Sets up the servo for steering
    void setupDCMotors();  // Sets up the GPIO pins for DC motor control
    void stopDCMotors();  // Stops both DC motors
    
    void asyncMove(std::function<void()> callback, int duration_ms);  // Helper function for asynchronous movement
    
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

