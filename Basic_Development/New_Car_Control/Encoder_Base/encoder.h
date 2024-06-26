#ifndef ENCODER_H
#define ENCODER_H

#include <pigpio.h>

typedef void (*encoderCallback_t)(int direction);

class Encoder {
public:
    Encoder(int gpioA, int gpioB, encoderCallback_t callback = nullptr);
    ~Encoder();
    int getPosition();
    void reset();
    int getDirection();

private:
    int gpioA, gpioB;
    int position;
    int direction;
    int levA, levB;
    int lastGpio;
    encoderCallback_t callback;

    void _pulse(int gpio, int level, uint32_t tick);
    static void _pulseEx(int gpio, int level, uint32_t tick, void *user);
};

#endif // ENCODER_H
