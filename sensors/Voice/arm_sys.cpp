#include "arm_sys.h"
#include <pigpio.h>
#include <iostream>
#include <thread>
#include <unistd.h>

bool mg90s::initialized = false;
int mg90s::instances = 0;

mg90s::mg90s(int gpioPin) : pin(gpioPin), currentAngle(25.0)
{
    if (!initialized)
    {
        if (gpioInitialise() < 0)
        {
            std::cerr << "pigpio initialization failed." << std::endl;
            exit(1);
        }
        initialized = true;
    }
    instances++;
}

mg90s::~mg90s()
{
    if (--instances <= 0)
    {
        gpioTerminate();
    }
}

void mg90s::setTargetAngleAsync(float angle, CallbackFunction callback)
{
    onTargetReached = callback;
    std::thread(&mg90s::detectAngle, this, angle).detach();
}

void mg90s::detectAngle(float angle)
{
    if (angle < -90)
    {
        angle = -90;
    }
    else if (angle > 90)
    {
        angle = 90;
    }
    moveToAngleSlowly(angle);
}

void mg90s::moveToAngleSlowly(float targetAngle)
{
    float step = (targetAngle > currentAngle) ? 1 : -1;

    while (currentAngle != targetAngle)
    {
        currentAngle += step;
        if ((step > 0 && currentAngle > targetAngle) || (step < 0 && currentAngle < targetAngle))
        {
            currentAngle = targetAngle;
        }
        int pulseWidth = angleToPulseWidth(currentAngle);
        gpioServo(pin, pulseWidth);
        usleep(20000);
    }

    if (onTargetReached)
    {
        onTargetReached();
    }
}

int mg90s::angleToPulseWidth(float angle)
{
    return static_cast<int>((angle + 90) * (2000.0 / 180.0) + 500);
}
