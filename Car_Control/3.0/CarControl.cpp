/***
 * author : Basav
 * last modified date: 14/04/2024
 * 
 * Basic 3.0, date: 13/04/2024
 * implementation of car control with feedback
 * 
 */
#include "CarControl.hpp"
#include <iostream>
#include <pigpio.h>
#include <thread>
#include <functional>
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;

const float speed_cm_per_sec_forward_backward = 5.0; // car's forward and backward speed in cm/s
const float speed_deg_per_sec_turn = 45.0; // car's turning speed in degrees per second

CarControl::CarControl() : _MS(0x60), _motorAPin1(17), _motorAPin2(22), _motorBPin1(10), _motorBPin2(11), _forwardBackwardSpeed(128), _turnSpeed(64), _servo(nullptr), _encoderPinA(5), _encoderPinB(13), _ppr(360), _wheelCircumference(31.4) {
}

CarControl::~CarControl() {
    instance = this;
    cleanup();
}

CarControl* CarControl::instance = nullptr;  // Initialize the static instance pointer

CarControl* CarControl::getInstance() {
    if (instance == nullptr) {
        instance = new CarControl();
    }
    return instance;
}

void CarControl::initialize() {
    if (gpioInitialise() < 0) {
        std::cerr << "Failed to initialize pigpio\n";
        exit(1);
    }
    setupServo();
    setupDCMotors();
    setupEncoders();
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

void CarControl::setupEncoders() {
    gpioSetMode(_encoderPinA, PI_INPUT);
    gpioSetPullUpDown(_encoderPinA, PI_PUD_UP);
    gpioSetISRFunc(_encoderPinA, EITHER_EDGE, 0, encoderISR);
    
    gpioSetMode(_encoderPinB, PI_INPUT);
    gpioSetPullUpDown(_encoderPinB, PI_PUD_UP);
    gpioSetISRFunc(_encoderPinB, EITHER_EDGE, 0, encoderISR);
}

// Static ISR handling encoder signals
void CarControl::encoderISR(int gpio, int level, uint32_t tick) {
    if (gpio == instance->_encoderPinA) {
        instance->_pulseCountA.fetch_add(1, std::memory_order_relaxed);
    } else if (gpio == instance->_encoderPinB) {
        instance->_pulseCountB.fetch_add(1, std::memory_order_relaxed);
    }
}

float CarControl::calculateDistance(int pulses) {
    return (pulses / static_cast<float>(_ppr)) * _wheelCircumference;
}

float CarControl::getDistanceTraveled() {
    int totalPulses = _pulseCountA.load(std::memory_order_relaxed) + _pulseCountB.load(std::memory_order_relaxed);
    return calculateDistance(totalPulses);
}

void CarControl::moveForward(float distance_cm, function<void()> callback) {
    int pulsesNeeded = static_cast<int>((distance_cm / _wheelCircumference) * _ppr);
    cout << "Initial Pulses: " << _pulseCountA.load() << endl;
    _pulseCountA = 0;
    gpioWrite(_motorAPin1, PI_HIGH);
    gpioWrite(_motorAPin2, PI_LOW);
    gpioWrite(_motorBPin1, PI_HIGH);
    gpioWrite(_motorBPin2, PI_LOW);
    thread([=]() {
        while (_pulseCountA.load() < pulsesNeeded);
        stopDCMotors();
        cout << "Final Pulses: " << _pulseCountA.load() << " for distance: " << distance_cm << " cm" << endl;
        if (callback) callback();
    }).detach();
}

void CarControl::moveBackward(float distance_cm, function<void()> callback) {
    int pulsesNeeded = static_cast<int>((distance_cm / _wheelCircumference) * _ppr);
    _pulseCountB = 0;
    gpioWrite(_motorAPin1, PI_LOW);
    gpioWrite(_motorAPin2, PI_HIGH);
    gpioWrite(_motorBPin1, PI_LOW);
    gpioWrite(_motorBPin2, PI_HIGH);
    thread([=]() {
        while (_pulseCountA.load() < pulsesNeeded);
        stopDCMotors();
        if (callback) callback();
    }).detach();
}

void CarControl::turnRight(int degrees, function<void()> callback) {
    int duration_ms = static_cast<int>((degrees / speed_deg_per_sec_turn) * 1000);
    if (_servo) {
        _servo->writeServo(120); // Adjust for a right turn
        this_thread::sleep_for(milliseconds(duration_ms)); // Maintain turn for the duration
        _servo->writeServo(90); // Reset to center position after the turn
    }
    if (callback) callback();
}

void CarControl::turnLeft(int degrees, function<void()> callback) {
    int duration_ms = static_cast<int>((degrees / speed_deg_per_sec_turn) * 1000);
    if (_servo) {
        _servo->writeServo(60); // Adjust for a left turn
        this_thread::sleep_for(milliseconds(duration_ms)); // Maintain turn for the duration
        _servo->writeServo(90); // Reset to center position after the turn
    }
    if (callback) callback();
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
