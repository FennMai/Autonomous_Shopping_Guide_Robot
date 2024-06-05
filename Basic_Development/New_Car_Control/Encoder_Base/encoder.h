/** 
 * 
 * @author FENN MAI
 * @date 06/05/2024
 * @version Basic 4.0
 * @brief 编码电机初次测试
 */

#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <pigpio.h>

typedef void (*encoderCallback_t)(int);

class Encoder {
public:
    Encoder(int gpioA, int gpioB, encoderCallback_t callback = nullptr);
    ~Encoder();
    int32_t getPosition();
    void reset();
    int getDirection(); // 新增方法，获取方向

private:
    int gpioA, gpioB;
    int32_t position;
    int lastGpio;
    int levA, levB;
    encoderCallback_t callback;
    int direction; // 记录方向

    static void _pulseEx(int gpio, int level, uint32_t tick, void *user);
    void _pulse(int gpio, int level, uint32_t tick);
};

#endif
