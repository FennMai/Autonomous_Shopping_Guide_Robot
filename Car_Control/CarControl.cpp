/***
 * author : Basav
 * last modified date: 10/04/2024
 * 
 * Basic 3.0, date: 10/04/2024
 * iplementation file for CarControl.hpp
 * 
 */
#include "CarControl.hpp"
#include <iostream>

CarControl::CarControl() : 
    _motorAPin1(5), //GPIO pin for Motor A IN1
    _motorAPin2(6), //GPIO pin for Motor A IN2
    _motorBPin1(16), //GPIO pin for Motor B IN1
    _motorBPin2(20), //GPIO pin for Motor B IN2
    _currentSpeed(128), // Adjust speed as needed
    _servo(nullptr) 
{
}

CarControl::~CarControl() {
    cleanup();
}

void CarControl::initialize() {
    if (gpioInitialise() < 0) {
        std::cerr << "Failed to initialize pigpio\n";
        exit(1);
    }
    setupServo();
    setupDCMotors();
}

void CarControl::setupServo() {
    Emakefun_MotorShield MS(0x60);
    MS.begin(50);
    _servo = MS.getServo(1);
    _servo->writeServo(90); // Center servo
}

void CarControl::setupDCMotors() {
    gpioSetMode(_motorAPin1, PI_OUTPUT);
    gpioSetMode(_motorAPin2, PI_OUTPUT);
    gpioSetMode(_motorBPin1, PI_OUTPUT);
    gpioSetMode(_motorBPin2, PI_OUTPUT);
}

void CarControl::moveForward(int duration) {
    gpioWrite(_motorAPin1, PI_HIGH);
    gpioWrite(_motorAPin2, PI_LOW);
    gpioWrite(_motorBPin1, PI_HIGH);
    gpioWrite(_motorBPin2, PI_LOW);
    gpioDelay(duration);
}

void CarControl::turnRight(int duration) {
    // Example: Adjust servo for a right turn. Angle and duration may need tweaking.
    _servo->writeServo(120);
    gpioDelay(duration);
    // Reset servo to center position if needed
}

void CarControl::turnLeft(int duration) {
    _servo->writeServo(60);
    gpioDelay(duration);
}

void CarControl::moveBackward(int duration) {
    gpioWrite(_motorAPin1, PI_LOW);
    gpioWrite(_motorAPin2, PI_HIGH);
    gpioWrite(_motorBPin1, PI_LOW);
    gpioWrite(_motorBPin2, PI_HIGH);
    gpioDelay(duration);
}

void CarControl::stop() {
    stopDCMotors();
    // Additional logic to handle servo can be added
}

void CarControl::setSpeed(int speed) {
    _currentSpeed = speed;
    // Optionally, adjust DC motor speeds here
}

void CarControl::stopDCMotors() {
    gpioWrite(_motorAPin1, PI_LOW);
    gpioWrite(_motorAPin2, PI_LOW);
    gpioWrite(_motorBPin1, PI_LOW);
    gpioWrite(_motorBPin2, PI_LOW);
}

void CarControl::cleanup() {
    stopDCMotors();
    gpioTerminate();
}
