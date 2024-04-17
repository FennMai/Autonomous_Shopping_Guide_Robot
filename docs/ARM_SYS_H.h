#ifndef ARM_SYS_H
#define ARM_SYS_H

#include <functional>
#include <Adafruit_PWMServoDriver.h>

extern Adafruit_PWMServoDriver pwm; // 声明，实际的定义在.cpp文件中

class mg90s {
public:
    using CallbackFunction = std::function<void()>;

    mg90s(int channel, float initialAngle = 0.0);
    ~mg90s();

    void setTargetAngleAsync(float angle, CallbackFunction callback = nullptr);

private:
    static bool initialized;   // 声明静态成员变量
    static int instances;      // 声明静态成员变量
    int channel;               // PWM channel on PCA9685
    float currentAngle;
    CallbackFunction onTargetReached;

    void moveToAngleSlowly(float targetAngle);
    int angleToPulseWidth(float angle);
    void detectAngle(float angle);
    void gpioServo(int channel, int pulseWidth);
};

#endif // ARM_SYS_H
