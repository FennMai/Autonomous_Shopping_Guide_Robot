#include "CarControl.hpp"
#include <pigpio.h>
#include <chrono>
#include <thread>

CarControl::CarControl(int motorPin1, int motorPin2, int enablePin, int servoPin)
    : _motorPin1(motorPin1), _motorPin2(motorPin2), _enablePin(enablePin), _servoPin(servoPin) {
}

CarControl::~CarControl() {
    gpioTerminate();
}

void CarControl::initialize() {
    if (gpioInitialise() < 0) {
        // Handle initialization failure
    }
    gpioSetMode(_motorPin1, PI_OUTPUT);
    gpioSetMode(_motorPin2, PI_OUTPUT);
    gpioSetMode(_enablePin, PI_OUTPUT);
    gpioSetMode(_servoPin, PI_OUTPUT);
    gpioSetPWMfrequency(_enablePin, 1000);
    gpioSetPWMrange(_enablePin, 255);
}

std::future<void> CarControl::moveForward(int distance, std::function<void(int)> callback) {
    return std::async(std::launch::async, &CarControl::move, this, distance, callback, true);
}

std::future<void> CarControl::moveBackward(int distance, std::function<void(int)> callback) {
    return std::async(std::launch::async, &CarControl::move, this, distance, callback, false);
}

void CarControl::turnLeft(int angle) {
    setServoAngle(angle);
}

void CarControl::turnRight(int angle) {
    setServoAngle(angle);
}

void CarControl::stop() {
    gpioWrite(_motorPin1, 0);
    gpioWrite(_motorPin2, 0);
    setMotorSpeed(0);
}

void CarControl::move(int distance, std::function<void(int)> callback, bool forward) {
    // Placeholder implementation: Calculate time based on distance and speed
    int speed = 100; // Speed placeholder
    int timeToMove = distance * 1000 / speed; // Simple calculation, needs adjustment based on actual speed and units

    if(forward) {
        gpioWrite(_motorPin1, 1);
        gpioWrite(_motorPin2, 0);
    } else {
        gpioWrite(_motorPin1, 0);
        gpioWrite(_motorPin2, 1);
    }

    setMotorSpeed(speed);
    std::this_thread::sleep_for(std::chrono::milliseconds(timeToMove)); // Simulate moving
    stop(); // Stop the car after moving

    if(callback) {
        callback(distance); // Call the callback function with the distance moved
    }
}

void CarControl::setMotorSpeed(int speed) {
    gpioPWM(_enablePin, speed);
}

void CarControl::setServoAngle(int angle) {
    int pulseWidth = angle * 2000 / 180 + 500;
    gpioServo(_servoPin, pulseWidth);
}

