/***
 * author : Basav
 * last modified date: 11/04/2024
 * 
 * Basic 3.0, date: 10/04/2024
 * iplementating car control with distance as a parameter instead of time(Delay)
 * 
 */
#include "CarControl.hpp"
#include <iostream>
#include <pigpio.h>

const float speed_cm_per_sec_forward_backward = 5.0; //car's forward and backward speed in cm/s
const float speed_deg_per_sec_turn = 45.0; //car's turning speed in degrees per second

CarControl::CarControl() : _MS(0x60), _motorAPin1(5), _motorAPin2(6), _motorBPin1(16), _motorBPin2(20), _forwardBackwardSpeed(128), _turnSpeed(64), _servo(nullptr) {
}

CarControl::~CarControl() {
    cleanup();
}

void CarControl::setupServo() {
    _MS.begin(50);
    _servo = _MS.getServo(1);
    _servo->writeServo(90); // Center servo
}

void CarControl::setupDCMotors() {
    gpioSetMode(_motorAPin1, PI_OUTPUT);
    gpioSetMode(_motorAPin2, PI_OUTPUT);
    gpioSetMode(_motorBPin1, PI_OUTPUT);
    gpioSetMode(_motorBPin2, PI_OUTPUT);
}

void CarControl::initialize() {
    if (gpioInitialise() < 0) {
        std::cerr << "Failed to initialize pigpio\n";
        exit(1);
    }
    setupServo();
    setupDCMotors();
}

void CarControl::moveForward(float distance_cm) {
    int duration_ms = static_cast<int>((distance_cm / speed_cm_per_sec_forward_backward) * 1000);
    gpioWrite(_motorAPin1, PI_HIGH);
    gpioWrite(_motorAPin2, PI_LOW);
    gpioWrite(_motorBPin1, PI_HIGH);
    gpioWrite(_motorBPin2, PI_LOW);
    gpioDelay(duration_ms * 1000);
}

void CarControl::moveBackward(float distance_cm) {
    int duration_ms = static_cast<int>((distance_cm / speed_cm_per_sec_forward_backward) * 1000);
    gpioWrite(_motorAPin1, PI_LOW);
    gpioWrite(_motorAPin2, PI_HIGH);
    gpioWrite(_motorBPin1, PI_LOW);
    gpioWrite(_motorBPin2, PI_HIGH);
    gpioDelay(duration_ms * 1000);
}

void CarControl::turnRight(int degrees) {
    int duration_ms = static_cast<int>((degrees / speed_deg_per_sec_turn) * 1000);
    if (_servo) {
        _servo->writeServo(120); // Adjust for a right turn
        gpioDelay(duration_ms * 1000); // Wait for the turn to complete
    }
    _servo->writeServo(90); // Reset to center position
}

void CarControl::turnLeft(int degrees) {
    int duration_ms = static_cast<int>((degrees / speed_deg_per_sec_turn) * 1000);
    if (_servo) {
        _servo->writeServo(60); // Adjust for a left turn
        gpioDelay(duration_ms * 1000); // Wait for the turn to complete
    }
    _servo->writeServo(90); // Reset to center position
}

void CarControl::setSpeed(int forwardBackwardSpeed, int turnSpeed) {
    _forwardBackwardSpeed = forwardBackwardSpeed;
    _turnSpeed = turnSpeed;
    // Note: Adjust the actual PWM values if necessary as per the hardware
}

void CarControl::stop() {
    stopDCMotors();
    // Additional logic to handle servo can be added here
}

void CarControl::stopDCMotors() {
    gpioWrite(_motorAPin1, PI_LOW);
    gpioWrite(_motorAPin2, PI_LOW);
    gpioWrite(_motorBPin1, PI_LOW);
    gpioWrite(_motorBPin2, PI_LOW);
}

void CarControl::cleanup() {
    stopDCMotors(); 
    gpioTerminate(); // Clean up the library
}
