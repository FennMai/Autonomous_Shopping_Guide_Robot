/***
 * author : Basav
 * last modified date: 14/04/2024
 * 
 * Basic 3.0, date: 13/04/2024
 * header for car control with distance measurement
 * 
 */
#ifndef CAR_CONTROL_HPP
#define CAR_CONTROL_HPP

#include "Emakefun_MotorShield.h"
#include <pigpio.h>
#include <functional>
#include <atomic>
#include <map>

class CarControl {
public:
    static CarControl* getInstance();  // Singleton access method
    void initialize();
    void moveForward(float distance_cm, std::function<void()> callback);
    void moveBackward(float distance_cm, std::function<void()> callback);
    void turnRight(int degrees, std::function<void()> callback);
    void turnLeft(int degrees, std::function<void()> callback);
    float getDistanceTraveled();
    void stop();
    void setSpeed(int forwardBackwardSpeed, int turnSpeed);
    void cleanup();

private:
    CarControl();  // Private constructor for singleton
    ~CarControl();

    void setupServo();
    void setupDCMotors();
    void setupEncoders();
    void stopDCMotors();
    float calculateDistance(int pulses);

    static CarControl* instance;
    static std::map<int, CarControl*> instances;
    static void encoderISR(int gpio, int level, uint32_t tick); // Static member function declaration

    void asyncMove(std::function<void()> callback, int duration_ms);

    int _motorAPin1;
    int _motorAPin2;
    int _motorBPin1;
    int _motorBPin2;
    int _encoderPinA;
    int _encoderPinB;
    int _forwardBackwardSpeed;
    int _turnSpeed;
    Emakefun_Servo* _servo;
    Emakefun_MotorShield _MS;
    std::atomic<int> _pulseCountA{0};
    std::atomic<int> _pulseCountB{0};
    int _ppr;
    float _wheelCircumference;
};

#endif // CAR_CONTROL_HPP

