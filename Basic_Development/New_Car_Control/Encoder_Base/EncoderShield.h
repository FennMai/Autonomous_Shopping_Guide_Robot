#ifndef _ENCODER_SHIELD_H_
#define _ENCODER_SHIELD_H_

#include "Emakefun_MotorDriver.h"
#include "encoder.h"

// 电机引脚定义
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6

// 电机方向定义
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

class EncoderShield;

class EncoderMotor {
public:
    EncoderMotor(void);
    void begin(uint8_t motorNum, uint8_t encA, uint8_t encB);
    void setSpeed(uint8_t speed);
    void run(uint8_t cmd);
    int32_t readEncoder();
    void resetEncoder();
    int getDirection(); // 获取方向

    void setMotorNum(uint8_t motorNum); // 添加设置电机编号的方法
    uint8_t getMotorNum() const; // 添加获取电机编号的方法
    EncoderShield *MC;

private:
    uint8_t motorNum, encAPin, encBPin, IN1pin, IN2pin, MDIR, _speed;
    volatile int32_t encoderPos;
    Encoder *encoder;
    
};

class EncoderShield {
public:
    EncoderShield(uint8_t addr = 0x60);
    void begin(uint16_t freq = 50);
    EncoderMotor *getEncoderMotor(uint8_t n);

    void setPWM(uint8_t pin, uint16_t val); ///< 设置PWM输出
    void setPin(uint8_t pin, uint8_t val); ///< 设置引脚输出

private:
    uint8_t _addr;
    uint16_t _freq;
    Emakefun_MotorDriver _pwm;
    EncoderMotor encoderMotors[4];
};

#endif
