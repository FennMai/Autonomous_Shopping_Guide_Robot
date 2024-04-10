#ifndef CARCONTROL_HPP
#define CARCONTROL_HPP

#include <functional> // For std::function
#include <future>     // For std::async, std::future

class CarControl {
public:
    CarControl(int motorPin1, int motorPin2, int enablePin, int servoPin);
    ~CarControl();
    void initialize();
    // Modified to move a certain distance with a callback on completion
    std::future<void> moveForward(int distance, std::function<void(int)> callback);
    std::future<void> moveBackward(int distance, std::function<void(int)> callback);
    void turnLeft(int angle);
    void turnRight(int angle);
    void stop();

private:
    int _motorPin1, _motorPin2, _enablePin, _servoPin;
    void setMotorSpeed(int speed);
    void setServoAngle(int angle);
    // Helper method for asynchronous movement
    void move(int distance, std::function<void(int)> callback, bool forward);
};

#endif

