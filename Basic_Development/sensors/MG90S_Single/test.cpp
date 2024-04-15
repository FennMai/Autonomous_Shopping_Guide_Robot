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
#include <unistd.h> // For sleep function

int main() {
    // Assuming the GPIO pins for the four motors are 17, 27, 22, and 10
    mg90s arm_mtr1(17, 0.0); // Initialize motor1 on GPIO pin 17, starting at angle 0
    mg90s arm_mtr2(27, 0.0); // Initialize motor2 on GPIO pin 27, starting at angle 0
    mg90s arm_mtr3(22, 0.0); // Initialize motor3 on GPIO pin 22, starting at angle 0
    mg90s arm_mtr4(10, 0.0); // Initialize motor4 on GPIO pin 10, starting at angle 0
    
    // callback
    auto onTargetReached = []() {
        std::cout << "Target angle reached." << std::endl;
    };

    // Asynchronously set target angles for each motor
    arm_mtr1.setTargetAngleAsync(45, onTargetReached); // Set motor1 to 45 degrees
    arm_mtr2.setTargetAngleAsync(-45, onTargetReached); // Set motor2 to -45 degrees
    arm_mtr3.setTargetAngleAsync(30, onTargetReached); // Set motor3 to 30 degrees
    arm_mtr4.setTargetAngleAsync(-30, onTargetReached); // Set motor4 to -30 degrees

    // Wait for a bit to let the motors reach their target angles
    sleep(5); // Sleep for 5 seconds

    // Further operations can go here...

    return 0;
}
