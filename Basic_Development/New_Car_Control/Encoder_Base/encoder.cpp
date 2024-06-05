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
Encoder::Encoder(int gpioA, int gpioB, encoderCallback_t callback) {
    this->gpioA = gpioA;
    this->gpioB = gpioB;
    this->callback = callback;
    position = 0;
    levA = 0;
    levB = 0;
    lastGpio = -1;
    direction = 0;

    // 将 gpioA 和 gpioB 引脚设置为输入模式
    gpioSetMode(gpioA, PI_INPUT);
    gpioSetMode(gpioB, PI_INPUT);
    // 启用了 gpioA 和 gpioB 引脚的上拉电阻。上拉电阻用于确保引脚在未连接或未驱动时具有确定的电平（在这里是高电平）。
    gpioSetPullUpDown(gpioA, PI_PUD_UP);
    gpioSetPullUpDown(gpioB, PI_PUD_UP);
    // gpioA 和 gpioB 引脚设置了一个回调函数 _pulseEx。
    // 当引脚的电平发生变化（从高到低或从低到高）时，将调用 _pulseEx 函数。
    // 通过这种方式，我们可以检测编码器的脉冲信号
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
    Encoder *mySelf = (Encoder *) user;
    mySelf->_pulse(gpio, level, tick);
}

// 实例脉冲回调函数
void Encoder::_pulse(int gpio, int level, uint32_t tick) {
    if (gpio == gpioA) levA = level; else levB = level;

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
int32_t Encoder::getPosition() {
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
