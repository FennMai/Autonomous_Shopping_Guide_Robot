/** 
 * 
 * @author FENN MAI
 * @date 06/05/2024
 * @version Basic 4.0
 * @brief 编码电机初次测试
 * 
 * 
 *              +---------+         +---------+         +---------+
A            |         |         |         |         |         |
   +---------+         +---------+         +---------+         +
       +---------+         +---------+         +---------+     
B      |         |         |         |         |         |     
   ----+         +---------+         +---------+         +-----

 */


#include "encoder.h"

// Encoder 构造函数
Encoder::Encoder(int gpioA, int gpioB, encoderCallback_t callback)
    : gpioA(gpioA), gpioB(gpioB), position(0), direction(0), levA(0), levB(0), lastGpio(-1), callback(callback) {
    gpioSetMode(gpioA, PI_INPUT);
    gpioSetMode(gpioB, PI_INPUT);
    gpioSetPullUpDown(gpioA, PI_PUD_UP);
    gpioSetPullUpDown(gpioB, PI_PUD_UP);
    gpioSetAlertFuncEx(gpioA, _pulseEx, this);
    gpioSetAlertFuncEx(gpioB, _pulseEx, this);
}

// Encoder 析构函数
Encoder::~Encoder() {
    gpioSetAlertFuncEx(gpioA, 0, this);
    gpioSetAlertFuncEx(gpioB, 0, this);
}

// 静态脉冲回调函数
void Encoder::_pulseEx(int gpio, int level, uint32_t tick, void *user) {
    Encoder *mySelf = static_cast<Encoder *>(user);
    mySelf->_pulse(gpio, level, tick);
}

// 实例脉冲回调函数
void Encoder::_pulse(int gpio, int level, uint32_t tick) {
    if (gpio == gpioA) levA = level;
    else levB = level;

    if (gpio != lastGpio) {
        lastGpio = gpio;

        if ((gpio == gpioA) && (level == 1)) {
            if (levB) {
                position++;
                direction = 1; // 顺时针
                if (callback) callback(1);
            }
        } else if ((gpio == gpioB) && (level == 1)) {
            if (levA) {
                position--;
                direction = -1; // 逆时针
                if (callback) callback(-1);
            }
        }
    }
}

// 获取编码器位置
int Encoder::getPosition() {
    return position;
}

// 重置编码器位置
void Encoder::reset() {
    position = 0;
}

// 获取编码器方向
int Encoder::getDirection() {
    return direction;
}
