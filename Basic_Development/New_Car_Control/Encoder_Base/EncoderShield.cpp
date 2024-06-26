/** 
 * 
 * @author FENN MAI
 * @date 06/05/2024
 * @version Basic 4.0
 * @brief 编码电机初次测试
 */

#include "EncoderShield.h"
#include <iostream>
#include <pigpio.h>
#include <algorithm>

// EncoderMotor 构造函数
EncoderMotor::EncoderMotor(void) 
    : motorNum(0), encAPin(0), encBPin(0), encoderPos(0), MC(nullptr), encoder(nullptr) {
}

// 初始化编码电机
void EncoderMotor::begin(uint8_t motorNum, uint8_t encA, uint8_t encB) {
    this->motorNum = motorNum;
    this->encAPin = encA;
    this->encBPin = encB;
    encoder = new Encoder(encA, encB);

    switch (motorNum) {
        case 1:
            IN1pin = 11;
            IN2pin = 13;
            break;
        case 2:
            IN1pin = 10;
            IN2pin = 8;
            break;
        case 3:
            IN1pin = 2;
            IN2pin = 4;
            break;
        case 4:
            IN1pin = 5;
            IN2pin = 7;
            break;
    }
}

// 设置电机速度
void EncoderMotor::setSpeed(uint8_t speed) {
    _speed = std::min(speed, static_cast<uint8_t>(255));
}

// 运行电机
void EncoderMotor::run(uint8_t cmd) {
    MDIR = cmd;
    uint16_t pwm_val = _speed * 16;  // Adjusted for the PCA9685 PWM range
    switch (cmd) {
        // BACKWARD：设置IN2pin为低电平（0），并为IN1pin设置PWM值。           
        case BACKWARD:
            std::cout << "Setting IN2pin: " << static_cast<int>(IN2pin) << " to 0" << std::endl;
            std::cout << "Setting PWM IN1pin: " << static_cast<int>(IN1pin) << " to " << pwm_val << std::endl;
            MC->setPin(IN2pin, 0);
            MC->setPWM(IN1pin, pwm_val);
            break;
        // FORWARD：设置IN1pin为低电平（0），并为IN2pin设置PWM值。
        case FORWARD:
            std::cout << "Setting IN1pin: " << static_cast<int>(IN1pin) << " to 0" << std::endl;
            std::cout << "Setting PWM IN2pin: " << static_cast<int>(IN2pin) << " to " << pwm_val << std::endl;
            MC->setPin(IN1pin, 0);
            MC->setPWM(IN2pin, pwm_val);
            
            break;
        // RELEASE：将IN1pin和IN2pin都设置为低电平（0），释放电机。
        case RELEASE:
            std::cout << "Releasing motor" << std::endl;
            MC->setPin(IN1pin, 0);
            MC->setPin(IN2pin, 0);
            break;
        // BRAKE：将IN1pin和IN2pin都设置为高电平（1），刹车。
        case BRAKE:
            std::cout << "Braking motor" << std::endl;
            MC->setPin(IN1pin, 1);
            MC->setPin(IN2pin, 1);
            break;
    }
}

// 读取编码器位置
int32_t EncoderMotor::readEncoder() {
    return encoder->getPosition();
}

// 重置编码器位置
void EncoderMotor::resetEncoder() {
    encoder->reset();
}

// 获取编码器方向
int EncoderMotor::getDirection() {
    return encoder->getDirection();
}

// 设置电机编号
void EncoderMotor::setMotorNum(uint8_t motorNum) {
    this->motorNum = motorNum;
}

// 获取电机编号
uint8_t EncoderMotor::getMotorNum() const {
    return motorNum;
}

// EncoderShield 构造函数
EncoderShield::EncoderShield(uint8_t addr)
    : _addr(addr), _freq(50), _pwm(Emakefun_MotorDriver(addr)) {
}

// 初始化电机驱动板
void EncoderShield::begin(uint16_t freq) {
    _pwm.begin();
    _freq = freq;
    _pwm.setPWMFreq(_freq);
    for (uint8_t i = 0; i < 16; i++) {
        _pwm.setPWM(i, 0, 0);
    }
}

// 设置PWM输出
void EncoderShield::setPWM(uint8_t pin, uint16_t val) {
    _pwm.setPWM(pin, 0, val);
}

// 设置引脚输出
void EncoderShield::setPin(uint8_t pin, uint8_t val) {
    _pwm.setPWM(pin, val == 0 ? 0 : 4096, 0);
}

// 获取编码电机对象
EncoderMotor *EncoderShield::getEncoderMotor(uint8_t num) {
    if (num > 4) return NULL;
    num--;
    if (encoderMotors[num].getMotorNum() == 0) {
        encoderMotors[num].setMotorNum(num + 1);
        encoderMotors[num].MC = this;
    }
    return &encoderMotors[num];
}
