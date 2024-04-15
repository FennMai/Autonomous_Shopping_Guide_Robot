/***
 * author : ZMai
 * last modified date: 28/03/2024
 * last commit: 57fa151
 * driving files: arm_sys.cpp, arm_sys.h, test.cpp
 * 
 * drive 4 mg90s, let them be controlled like a system(basic 2.0,reference arm_sys.cpp)
 * goal:use the Async method, motor running step by step (basic 2.1)
 * 
 * basic 2.1, date:20/03/2024 :
 * fix the error, when run the setTargetAngleAsync, motor will set 0 angle every time
 * 
 * sys 2.3, date:25/03/2024,commit: 57fa151:
 * drive the system, define the initial joints position
 * use terminal input the 4 angles to contorl the arm system, only use this file.
 * 
 * sys 2.4, date:28/03/2024
 * package arm system(async), test.cpp can use it.
 * nest step: async callback 
 */


#include "arm_sys.h"
#include <iostream>
#include <pigpio.h>
#include <vector>
#include <thread>
#include <functional>
#include <sstream>
#include <string>
#include <unistd.h>

// Initialize static member variables
bool mg90s::initialized = false;
int mg90s::instances = 0;

// Constructor
mg90s::mg90s(int gpioPin, float initialAngle) : pin(gpioPin), currentAngle(initialAngle) {
    // Initialize the pigpio library if it hasn't been initialized yet
    if (!initialized) {
        if (gpioInitialise() < 0) {
            std::cerr << "pigpio initialization failed." << std::endl;
            exit(1);
        }
        initialized = true;
    }
    instances++;

    // Optionally: Move the servo to its initial position
    gpioServo(pin, angleToPulseWidth(currentAngle));
}

// Destructor
mg90s::~mg90s() {
    // Terminate the pigpio library if this is the last instance
    if (--instances <= 0) {
        gpioTerminate();
    }
}

// Asynchronously set the target angle
void mg90s::setTargetAngleAsync(float angle, CallbackFunction callback) {
    onTargetReached = callback; // Store the callback function
    std::thread(&mg90s::detectAngle, this, angle).detach(); // Create and detach a thread to move the servo asynchronously
}

// Angle pre-processing
void mg90s::detectAngle(float angle) {
    if (angle < -90) {
        angle = -90;
    } else if (angle > 90) {
        angle = 90;
    }
    moveToAngleSlowly(angle);
}

// Private method to smoothly move to the target angle
void mg90s::moveToAngleSlowly(float targetAngle) {
    float step = (targetAngle > currentAngle) ? 1 : -1; // Determine the step direction

    while (currentAngle != targetAngle) {
        currentAngle += step;
        if ((step > 0 && currentAngle > targetAngle) || (step < 0 && currentAngle < targetAngle)) {
            currentAngle = targetAngle; // Prevent overshooting
        }
        int pulseWidth = angleToPulseWidth(currentAngle);
        gpioServo(pin, pulseWidth); // Set the servo angle
        usleep(20000); // 20ms delay to simulate smooth movement
    }

    if (onTargetReached) {
        onTargetReached(); // Call the callback function after the movement is complete
    }
}

// Convert angle to PWM pulse width
int mg90s::angleToPulseWidth(float angle) {
    return static_cast<int>((angle + 90) * (2000.0 / 180.0) + 500);
}





