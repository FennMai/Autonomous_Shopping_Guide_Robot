/***
 * author : Basav
 * last modified date: 14/04/2024
 * 
 * Basic 3.0, date: 10/04/2024
 * header for car control with more smooth control and precise movements
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
    static CarControl* getInstance();
    void initialize();
    void cleanup();
    void moveForward(float distance_cm, std::function<void()> callback);
    void moveBackward(float distance_cm, std::function<void()> callback);
    void turnRight(int degrees, std::function<void()> callback);
    void turnLeft(int degrees, std::function<void()> callback);
    void stop();
    void applyMotorSpeed();
    void setSpeed(int forwardBackwardSpeed, int turnSpeed);
    float getDistanceTraveled() const;
    float getXPosition() const;  // Get the current X position
    float getYPosition() const;  // Get the current Y position
    float getCurrentAngle() const;  // Get the current orientation angle in degrees
    float calculateDistance(int pulses) const;

    static void encoderISR(int gpio, int level, uint32_t tick);

private:
    CarControl();
    ~CarControl();
    void setupServo();
    void setupDCMotors();
    void setupEncoders();
    void stopDCMotors();
    Emakefun_MotorShield _MS;
    Emakefun_Servo* _servo;

    static CarControl* instance;
    int _motorAPin1, _motorAPin2, _motorBPin1, _motorBPin2;
    int _encoderPinA, _encoderPinB;
    std::atomic<int> _pulseCountA{0}, _pulseCountB{0};
    int _ppr;
    float _wheelCircumference;
    int _forwardBackwardSpeed, _turnSpeed;
    static int map(int x, int in_min, int in_max, int out_min, int out_max);
    float _xPos, _yPos;  // Position coordinates
    float _heading;  // Current heading in degrees
};

#endif // CAR_CONTROL_HPP
