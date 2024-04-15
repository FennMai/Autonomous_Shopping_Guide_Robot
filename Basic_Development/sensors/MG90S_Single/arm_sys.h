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

#ifndef ARM_SYS_H
#define ARM_SYS_H

#include <functional> // For std::function

class mg90s
{
public:
    using CallbackFunction = std::function<void()>;

    // define the input 
    mg90s(int gpioPin, float initialAngle = 0.0);

    ~mg90s();

    void setTargetAngleAsync(float angle, CallbackFunction callback = nullptr);

private:
    static bool initialized;
    static int instances;
    int pin;
    float currentAngle; // No need to change this
    CallbackFunction onTargetReached;

    void moveToAngleSlowly(float targetAngle);
    int angleToPulseWidth(float angle);
    void detectAngle(float angle); 
};

#endif // ARM_SYS_H
