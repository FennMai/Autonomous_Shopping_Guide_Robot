#include "mg90s.h"
#include <iostream>
#include <vector>
#include <thread>
#include <unistd.h>

// 初始化外部变量
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// 初始化静态成员变量
bool mg90s::initialized = false;
int mg90s::instances = 0;

// Constructor implementation
mg90s::mg90s(int channel, float initialAngle) : channel(channel), currentAngle(initialAngle) {
    if (!initialized) {
        pwm.begin();
        pwm.setPWMFreq(50);  // Set to servo standard 50 Hz refresh rate
        initialized = true;
    }
    instances++;
    gpioServo(channel, angleToPulseWidth(currentAngle));
}

// Destructor implementation
mg90s::~mg90s() {
    if (--instances <= 0) {
        pwm.reset();
        initialized = false;
    }
}

// Asynchronously set the target angle
void mg90s::setTargetAngleAsync(float angle, CallbackFunction callback) {
    onTargetReached = callback;
    std::thread(&mg90s::detectAngle, this, angle).detach();
}

// Detect and clamp the angle before moving
void mg90s::detectAngle(float angle) {
    if (angle < -90) {
        angle = -90;
    } else if (angle > 90) {
        angle = 90;
    }
    moveToAngleSlowly(angle);
}

// Smoothly move to the target angle
void mg90s::moveToAngleSlowly(float targetAngle) {
    float step = (targetAngle > currentAngle) ? 1 : -1;

    while (currentAngle != targetAngle) {
        currentAngle += step;
        if ((step > 0 && currentAngle > targetAngle) || (step < 0 && currentAngle < targetAngle)) {
            currentAngle = targetAngle;
        }
        int pulseWidth = angleToPulseWidth(currentAngle);
        gpioServo(channel, pulseWidth);
        usleep(20000);  // 20ms delay to simulate smooth movement
    }

    if (onTargetReached) {
        onTargetReached();
    }
}

// Convert angle to PWM pulse width
int mg90s::angleToPulseWidth(float angle) {
    float pulseLength = ((angle + 90) * (2000 - 500) / 180 + 500);  // Map from -90 to +90 to 500 to 2500
    int pulseWidth = static_cast<int>((pulseLength * 4096 / 20000));  // Map to 0-4095 for PCA9685
    return pulseWidth;
}

// Set servo PWM via PCA9685
void mg90s::gpioServo(int channel, int pulseWidth) {
    int on = 0;  // Start of the PWM pulse
    pwm.setPWM(channel, on, pulseWidth);
}

// Additional functions and logic can be added as required
