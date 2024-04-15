/***
 * author : Basav
 * last modified date: 15/04/2024
 * 
 * Basic 3.0, date: 14/04/2024
 * header for car control to move to a desired position (_xPos, _yPos) 
 * 
 */
#ifndef CAR_CONTROL_HPP
#define CAR_CONTROL_HPP

#include "Emakefun_MotorShield.h"
#include <pigpio.h>
#include <functional>
#include <cmath>
#include <atomic>

class CarControl {
public:
    static CarControl* getInstance(); // Singleton accessor

    void initialize();
    void cleanup();
    
    //Movement methods
    void moveForward(float distance_cm, std::function<void()> callback);
    void moveBackward(float distance_cm, std::function<void()> callback);
    void turnRight(int degrees, std::function<void()> callback);
    void turnLeft(int degrees, std::function<void()> callback);
    void navigateToPoint(float xTarget, float yTarget);  // method to move to point
    void stop();
    

    // Setters
    void setSpeed(int forwardBackwardSpeed, int turnSpeed);
    void applyMotorSpeed();

    // Getters
    float getXPosition() const;
    float getYPosition() const;
    float getCurrentAngle() const;
    float getDistanceTraveled() const;


    float calculateDistance(int pulses) const;

    static void encoderISR(int gpio, int level, uint32_t tick);

private:
    CarControl();
    ~CarControl();

    void setupServo();
    void setupDCMotors();
    void setupEncoders();
    void stopDCMotors();

    // Motor Shield and Servo
    Emakefun_MotorShield _MS;
    Emakefun_Servo* _servo;

    static CarControl* instance;

    // Motor and servo control
    int _motorAPin1, _motorAPin2, _motorBPin1, _motorBPin2;
    int _encoderPinA, _encoderPinB;
    float _xPos, _yPos;  // Position coordinates
    float _heading;  // Current heading in degrees
    std::atomic<int> _pulseCountA{0}, _pulseCountB{0};
    int _ppr;  // Pulses per revelution
    float _wheelCircumference;
    int _forwardBackwardSpeed, _turnSpeed;

    // Helper function to map values
    static int map(int x, int in_min, int in_max, int out_min, int out_max);
    
};

#endif // CAR_CONTROL_HPP
