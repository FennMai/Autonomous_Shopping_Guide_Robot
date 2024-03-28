#ifndef MG90S_H
#define MG90S_H

#include <functional> // 用于std::function

class mg90s
{
public:
    using CallbackFunction = std::function<void()>;

    mg90s(int gpioPin);
    ~mg90s();

    void setTargetAngleAsync(float angle, CallbackFunction callback = nullptr);

private:
    static bool initialized;
    static int instances;
    int pin;
    float currentAngle;
    CallbackFunction onTargetReached;

    void detectAngle(float angle);
    void moveToAngleSlowly(float targetAngle);
    int angleToPulseWidth(float angle);
};

#endif // MG90S_H
